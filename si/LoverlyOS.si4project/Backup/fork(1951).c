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
	unsigned long struct_tcb;
	unsigned long user_stack;
	int pid;
	
	struct_tcb = get_free_pages(KERNEL_STACK_PAGES);
	pid = ++last_pid;

	printk("struct_tcb[%d]=%08x\n", pid, struct_tcb);

	p = (struct task_struct *)struct_tcb;
	p->pid    = pid;
	p->state  = TASK_RUNNING;
	p->delay  = 0;
	p->u_time = 0;
	p->k_time = 0;
	p->prev   = 0x0;
	p->next   = 0x0;
	/* alloc memory block to new task kernel stack */
	p->thread.esp0 = struct_tcb+KERNEL_STACK_PAGES*PAGE_SIZE;
	p->thread.ss0 = SELECTOR_KERNEL_DATA;
	p->thread.eip = (unsigned long)ret_from_fork;
	p->thread.ss  = SELECTOR_USER_DATA;	
	p->thread.trace_bitmap =  0x80000000;
	/* copy current task kernel stack to new task kernel stack */
	childreg = (struct regs *)((p->thread.esp0) - (sizeof(struct regs)));
	reg_copy(childreg, (void*)(stack_start+4), sizeof(struct regs));
	p->thread.esp = (unsigned long)childreg;

	/*  child task EAX=0 */ 
	childreg->eax = 0;
	
	/* alloc memory block to new task user stack */
	// childreg->esp = get_free_pages(USER_STACK_PAGES) + USER_STACK_SIZE;
	user_stack = get_free_pages(USER_STACK_PAGES);
	printk("task[%d] user stack %08x\n",pid,childreg->esp);

	/* alloc page global directory */
	p->pgd = get_free_page();
	if ( !p->pgd ){
		printk("FILE %s,LINE %d\n",__FILE__,__LINE__);
		while(1);
	}
	mm_copy(current, p);

	uint32 *dir = (uint32*)p->pgd;
	for ( int i = 0; i < 4; i++ )
	{
		printk("i = %08x\n",dir[i]);
	}
	
	memcpy((char*)user_stack,(char*)current->stack_button,USER_STACK_PAGES*PAGE_SIZE);
	for ( int i = 0; i < USER_STACK_PAGES; i++ ){
		 __page_map( p->pgd, user_stack+i*PAGE_SIZE, current->stack_button+i*PAGE_SIZE, P_P|P_RW|P_US );
	}
	
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
		*(pdir + pdir_entry )  = ((uint32)ptable & 0xFFFFF000) + attr;
	}else{
		ptable = (uint32*)(*(pdir + pdir_entry ) & 0xFFFFF000);
	}

	*(ptable+ppte_entry) = (pa & 0xFFFFF000) + attr;

	return (0);
}



