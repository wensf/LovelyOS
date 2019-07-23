#include <types.h>
#include <sched.h>
#include <buffer.h>
#include <libc.h>
#include <page.h>
#include <slab.h>
#include <traps.h>

extern void ret_from_fork(void);

int printk(const char *fmt, ...);

static int last_pid;

void reg_copy(void *to, void *from, int cnt)
{
	memcpy(to, from, cnt);
}

void kernel_stack_dump(struct regs *reg)
{
	printk("eax = %08x\n", reg->eax);
	printk("ebx = %08x\n", reg->ebx);
	printk("ecx = %08x\n", reg->ecx);
	printk("edx = %08x\n", reg->edx);
	printk("esi = %08x\n", reg->esi);
	printk("edi = %08x\n", reg->edi);
	printk("ebp = %08x\n", reg->ebp);
	printk("fs  = %04x\n", reg->fs);
	printk("gs  = %04x\n", reg->gs);
	printk("es  = %04x\n", reg->es);
	printk("ds  = %04x\n", reg->ds);
	printk("eip = %08x\n", reg->eip);
	printk("cs  = %04x\n", reg->cs);
	printk("eflags = %08x\n", reg->eflags);
	printk("esp = %08x\n", reg->esp);
	printk("ss  = %04x\n", reg->ss);
}

void mm_copy(const struct task_struct *old, struct task_struct *new)
{
	uint32 *pgd_old, *pgd_new;

	pgd_old = (uint32 *)old->pgd;
	pgd_new = (uint32 *)new->pgd;

	printk("parent pgd %08x,child pgd %08x\n",(uint32)pgd_old,(uint32)pgd_new);
	
	for ( int i = 0; i < 4; i++ )  // copy 4MB kernel space page table
	{
		pgd_new[i] = pgd_old[i];	
	//	printk("%d,%08x,%08x\n",i,pgd_new[i],pgd_old[i]);
	}

	for ( int i = 4; i < 1024; i++ ) // user space page table not initiaze now
	{
		pgd_new[i] = pgd_old[i];	
	//	printk("%d,%08x,%08x\n",i,pgd_new[i],pgd_old[i]);
	}
}

/**
 * the one just the return address for sys_call0_for()
 * int do_fork(int nr,unsigned long stack_start)
 */
int do_fork(unsigned long stack_start)
{
	struct task_struct *p;
 	struct regs *childreg;
	int pid;

	/**
	 * 分配KERNEL_STACK_PAGES作为内核栈,并在栈底建立TCB结构
	 * 新的TCP结构复制了父进程几乎所有的内容,除了EAX, 内核栈指针ESP
	 * 为了简单起见,我们暂时未实现COW技术,fork时直接分配用户栈,并复制
	 * 父进程用户栈的内容.
	 */
	
	p = (struct task_struct *)get_free_pages(KERNEL_STACK_PAGES);
	*p = *current;
	pid = ++last_pid;

	printk("struct_tcb[%d]=%08x, task[%d] fork():\n", pid, (uint32)p, current->pid);
	
	p->pid    = pid;
	p->state  = TASK_INTERRUPTABLE;
	p->delay  = 0;
	p->u_time = 0;
	p->k_time = 0;
	p->last_fd = 0;//current->last_fd;
	for ( int i = 0; i < FSAL_MAX_OPENED_FILE; i++){
		p->file[i] = current->file[i];
	}
	/* alloc memory block to new task kernel stack */
	p->thread.esp0 = (uint32)p + KERNEL_STACK_PAGES*PAGE_SIZE -4;
	p->thread.ss0 = SELECTOR_KERNEL_DATA;
	p->thread.eip = (unsigned long)ret_from_fork;
	p->thread.ss  = SELECTOR_USER_DATA;	
	p->thread.trace_bitmap =  0x80000000;
	
	/* copy current task kernel stack to new task kernel stack */
	childreg = (struct regs *)((p->thread.esp0) - (sizeof(struct regs)));
	reg_copy(childreg, (void*)(stack_start+4), sizeof(struct regs));

	// child process diffrent with parent
	p->thread.esp = (unsigned long)childreg;
	childreg->eax = 0;
	p->parent = current;
	p->mm.stack_start = current->mm.stack_start; // 和父进程使用相同地址的栈空间.

	#if 1

	#if 1
	// 将父进程用户栈设置为只读,并将页面计数+1,当父子进程任何一个进行写入操作时，将page_default，从而重新分配页面.
 	for ( int i = 0; i < USER_STACK_PAGES; i++ )
	{
		unsigned long vaddr = current->mm.stack_start->vaddr+i*PAGE_SIZE;
		if (page_attrs_set(current->pgd,vaddr,P_P|P_US) != 0 )
		{
			kernel_die("page_attrs_set [0x%08x] failed\n", vaddr);
		}else{
			printk("page_attrs_set [0x%08x] to 0x%08x\n", vaddr, P_P|P_US);	
		}	
		#if 1
		if ( vaddr >= low_memory_start ){
			mem_map[MAP_NR(vaddr)]++;
		}
		#endif
 	}
	#endif

	/* 为子进程分配页目录,并复制父进程的页表项 */
	p->pgd = get_free_page();
	if ( !p->pgd ){
		kernel_die("fork() get_free_page failed\n");
	}
	mm_copy(current, p);		
	

#if 0 // Copy the parent stack virtual space
	struct vmb *p_vmb;

	p_vmb = current->mm.stack_start;
	while (p_vmb)
	{
		struct vmb *vmb = (struct vmb *)slab_alloc(sizeof(struct vmb));
		vmb->attrs = P_P|P_US;
		vmb->count = 0;
		vmb->length = PAGE_SIZE;
		vmb->vaddr = p_vmb->vaddr;
		
		vmb->next = p->mm.stack_start;
		p->mm.stack_start = vmb;

		uint32 vaddr = p_vmb->vaddr;
		if (page_attrs_set(p->pgd,vaddr,P_P|P_US) != 0 )
		{
			kernel_die("page_attrs_set [%08x] failed\n", vaddr);
		}		

		p_vmb = p_vmb->next;
	}
#endif
	
	#endif

	p->state  = TASK_RUNNING;
	task[pid] = p;

	// task_dump(p);

	// printk("task[%d]->last_fd=%d\n", pid, p->last_fd);
	
	return pid;
}


