
#include <sched.h>
#include <buffer.h>
#include <libc.h>
#include <memory.h>

extern void ret_from_fork(void);

int printk(const char *fmt, ...);

extern int last_pid;

struct regs
{
	unsigned long eax;
	unsigned long ebx;
	unsigned long ecx;
	unsigned long edx;
	unsigned long esi;
	unsigned long edi;
	unsigned long ebp;
	unsigned long fs;
	unsigned long gs;
	unsigned long ds;
	unsigned long es;
	unsigned long error; /* as a error */
	unsigned long eip;
	unsigned long cs;
	unsigned long eflags;
	unsigned long esp;
	unsigned long ss;
};

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

/**
 * the one just the return address for sys_call0_for()
 * int do_fork(int nr,unsigned long stack_start)
 */
int do_fork(int nr,unsigned long stack_start)
{
	struct task_struct *p;
	struct regs *childreg;
	unsigned long struct_tcb;
	
	struct_tcb = get_free_pages(KERNEL_STACK_PAGES);
	
	printk("struct_tcb[%d]=%08x\n", nr, struct_tcb);

	p = (struct task_struct *)struct_tcb;
	p->pid    = last_pid;
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
	childreg->esp = get_free_pages(USER_STACK_PAGES) + USER_STACK_SIZE;

	task[nr] = p;

	return last_pid;
}

int find_empty_process(void)
{
	return ++last_pid;
}

