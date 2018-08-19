
#include <sched.h>
#include <buffer.h>
#include <libc.h>
#include <memory.h>

extern void ret_from_fork(void);

int printk(const char *fmt, ...);

extern int last_pid;

void task_struct_dump(struct task_struct *p)
{
#if 0
	printk("pid = %d,state = %d,eip = %08x\n", p->pid,p->state,p->thread.eip);
#endif
}

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

void pt_copy(void *to, void *from, int cnt)
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
 */
//int copy_process(int nr, long ebp, long edi, long esi, long gs,
//			long none, long ebx, long ecx,
//			long edx, long fs,long es, long ds,
//			long eip, long cs, long eflags, long esp, long ss)
int do_fork(int nr,unsigned long stack_start)
{
	struct task_struct *p;
	struct regs *childreg;

	p = (struct task_struct *)&buff[nr];
	p->pid = last_pid;
	p->state = TASK_RUNNING;
	p->delay = 0;
	p->thread.esp0 = (unsigned long)kernel_stack[nr] + (1024*8);
	//p->tss.ss0 = SELECTOR_KERNEL_DATA;
	p->thread.eip = (unsigned long)ret_from_fork;
	//p->thread.esp = p->tss.esp0;
	p->thread.trace_bitmap =  0x80000000;

	childreg = (struct regs *)((p->thread.esp0) - (sizeof(struct regs)));

	pt_copy(childreg, (void*)(stack_start+4), sizeof(struct regs));

	p->thread.esp = (unsigned long)childreg;
	childreg->eax = 0;
	childreg->esp = (unsigned long)user_stack[nr] + (1024*8);

	task[nr] = p;

	#if 0
	task_struct_dump(p);
	kernel_stack_dump(childreg);
	printk("child_reg address : %08x\n", childreg);
	printk("thread.esp %08x\n", p->thread.esp);

	printk("task[%d] kernel mode sta top %08x\n",nr, p->thread.esp0);
	printk("task[%d] user mode stack top %08x\n",nr, childreg->esp);
	printk("do_fork nr = %d, stack_start = %08x, tcp addr %08x\n", nr, stack_start, p);
	#endif

	return last_pid;
}

int find_empty_process(void)
{
	/*int i = -1;

	printk("find_empty_process\n");

	repeat:
		if((++last_pid) < 0) last_pid = 1;
		for( i = 0; i < TASK_NR; i++)
		{
			if( task[i]->pid == last_pid ) goto repeat;
		}
	for( i = 1; i < TASK_NR; i++)
	{
		if(!task[i]) break;//return i;
	}
	printk("i = %d last_pid = %d\n", i,last_pid);

	return i;*/

	int i;

	i = ++last_pid;

	return i;
}
