#include <types.h>
#include <sched.h>
#include <buffer.h>
#include <libc.h>
#include <memory.h>
#include <page.h>

extern void ret_from_fork(void);

int printk(const char *fmt, ...);

extern int last_pid;

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

	printk("pdir %08x,%08x\n",(uint32)pgd_old,(uint32)pgd_new);
	
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

int __page_map ( uint32 pgd, uint32 pa, uint32 va, int attr );

/**
 * the one just the return address for sys_call0_for()
 * int do_fork(int nr,unsigned long stack_start)
 */
int do_fork(unsigned long stack_start)
{
	struct task_struct *p;
 	struct regs *childreg;
	unsigned long user_stack;
	int pid;

	/**
	 * 分配KERNEL_STACK_PAGES作为内核栈,并在栈底建立TCB结构
	 * 新的TCP结构复制了父进程几乎所有的内容,除了EAX, 内核栈指针ESP
	 * 为了简单起见,我们暂时未实现COW技术,fork时直接分配用户栈,并复制
	 * 父进程用户栈的内容.
	 */
	
	p = (struct task_struct *)get_free_pages(KERNEL_STACK_PAGES);
	pid = ++last_pid;

	printk("struct_tcb[%d]=%08x\n", pid, (uint32)p);
	
	p->pid    = pid;
	p->state  = TASK_RUNNING;
	p->delay  = 0;
	p->u_time = 0;
	p->k_time = 0;
	p->prev   = 0x0;
	p->next   = 0x0;
	/* alloc memory block to new task kernel stack */
	p->thread.esp0 = (uint32)p + KERNEL_STACK_PAGES*PAGE_SIZE;
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

	#if 1
	/* 为子进程分配用户栈 */
	user_stack = get_free_pages(USER_STACK_PAGES);
	printk("task[%d] user stack %08x\n",pid,user_stack+USER_STACK_PAGES*PAGE_SIZE);

	/* 为子进程分配页目录,并复制父进程的页表项 */
	p->pgd = get_free_page();
	if ( !p->pgd ){
		printk("FILE %s,LINE %d\n",__FILE__,__LINE__);
		while(1);
	}
	mm_copy(current, p);

	uint32 *dir = (uint32*)p->pgd;
	for ( int i = 0; i < 4; i++ )
	{
	//	printk("i = %08x\n",dir[i]);
	}
	#if 0
	memcpy((char*)user_stack,(char*)current->stack_button,USER_STACK_PAGES*PAGE_SIZE);
	for ( int i = 0; i < USER_STACK_PAGES; i++ ){
		 __page_map( p->pgd, user_stack+i*PAGE_SIZE, current->stack_button+i*PAGE_SIZE, P_P|P_RW|P_US );
	}
	#endif
	#endif

	#if 0
	p->next = task[0]->prev;
	p->prev = current;
	current->next = p;
	#endif

	task[pid] = p;
	
	return pid;
}

int __page_map ( uint32 pgd, uint32 pa, uint32 va, int attr )
{
	uint32 *pdir = (uint32 *)pgd;
	uint32 *ptable;

	uint32 pdir_entry = va >> 22;
	uint32 ppte_entry = (va >> 12 ) & 0x3FF;

	if ( *(pdir + pdir_entry ) == 0 )
	{
		ptable = (uint32 *) get_free_page();
		printk("page_map:alloc table %08x\n", (uint32)ptable);
		*(pdir + pdir_entry )  = ((uint32)ptable & 0xFFFFF000) + attr;
	}else{
		ptable = (uint32*)(*(pdir + pdir_entry ) & 0xFFFFF000);
	}

	*(ptable+ppte_entry) = (pa & 0xFFFFF000) + attr;

	printk("pgd=%08x,dir=[%d],pte=[%d],pa=%08x,va=%08x\n",pgd, pdir_entry,ppte_entry,pa,va);

	return (0);
}


void do_page_fault(void)
{
	uint32 va;
	__asm__ __volatile__ ("movl %%cr2,%%eax":"=a"(va));

	printk("task[%d] page_fault va at %08x\n", current->pid, va);

	va &= 0xFFFFF000;
	// 申请一块内存

	// 判断一下va是属于哪类地址: 0内核空间(),1用户空间,2其他
    // 子类：代码，数据，栈
    // 用户空间：代码：分配新页，并从RAM_DISK读取一页
    //           数据: 分配新页，并复制其父进程的页内容
    //           栈：  分配新页


	uint32 pa = get_free_page();
	
	__page_map( current->pgd, pa, va, P_P|P_RW|P_US );

	if ( va >= 0x1000000 ){		// 16MB

		// 用户空间缺页
		
		
		if ( va >= 0x4000000 ){	// >=64MB 堆栈空间
			// 从RAM_DISK内存段中载入新进程的代码.先这样做吧，后续实现文件系统以后，改为从文件系统中读取可执行文件.
			// 此处新进程由loader加载在主内存临时区8M附近.
			printk("on user stack\n");
			memset(pa, 0, PAGE_SIZE);
		}else{					// >=32<64MB 代码空间
			uint32 offset = 0x800000+120*512; // 程序入口在偏移120*512处
			printk("on user text\n");
			memcpy((int8*)pa,(int8*)offset,PAGE_SIZE);
 		}
	}else{
		// 内核空间缺页
		printk("on kernel\n");
		while(1);
	}
}


