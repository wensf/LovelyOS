
#include <sched.h>
#include <buffer.h>
#include <libc.h>
#include <memory.h>

extern void ret_from_fork(void);

int printf(const char *fmt, ...);

extern int last_pid;

void task_struct_dump(struct task_struct *p)
{
#if 0
	printf("pid %d\n", p->pid);
	printf("tss.back_link = %d\n", p->tss.back_link);
	printf("tss.esp0 = %08x\n", p->tss.esp0);
	printf("tss.ss0 = %08x\n", p->tss.ss0);
	printf("tss.eip = %08x\n", p->tss.eip);
	printf("tss.eflags = %08x\n", p->tss.eflags);
	printf("tss.eax = %08x\n", p->tss.eax);
	printf("tss.ecx = %08x\n", p->tss.ecx);
	printf("tss.edx = %08x\n", p->tss.edx);
	printf("tss.ebx = %08x\n", p->tss.ebx);
	printf("tss.esp = %08x\n", p->tss.esp);
	printf("tss.ebp = %08x\n", p->tss.ebp);
	printf("tss.esi = %08x\n", p->tss.esi);
	printf("tss.edi = %08x\n", p->tss.edi);
	printf("tss.es = %08x\n", p->tss.es);
	printf("tss.cs = %08x\n", p->tss.cs);
	printf("tss.ss = %08x\n", p->tss.ss);
	printf("tss.ds = %08x\n", p->tss.ds);
	printf("tss.fs = %08x\n", p->tss.fs);
	printf("tss.gs = %08x\n", p->tss.gs);
	printf("tss.trace_bitmap = %08x\n", p->tss.trace_bitmap);
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
	unsigned long ds;
	unsigned long es;
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
	printf("eax = %08x\n", reg->eax);
	printf("ebx = %08x\n", reg->ebx);
	printf("ecx = %08x\n", reg->ecx);
	printf("edx = %08x\n", reg->edx);
	printf("esi = %08x\n", reg->esi);
	printf("edi = %08x\n", reg->edi);
	printf("ebp = %08x\n", reg->ebp);
	printf("ds  = %04x\n", reg->ds);
	printf("es  = %04x\n", reg->es);
	printf("eip = %08x\n", reg->eip);
	printf("cs  = %04x\n", reg->cs);
	printf("eflags = %08x\n", reg->eflags);
	printf("esp = %08x\n", reg->esp);
	printf("ss  = %04x\n", reg->ss);
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

	printf("do_fork nr = %d, stack_start = %08x\n", nr, stack_start);

	p = (struct task_struct *)&buff[nr];
	p->pid = last_pid;
	p->thread.esp0 = (unsigned long)kernel_stack[nr] + (64*4);
	//p->tss.ss0 = SELECTOR_KERNEL_DATA;
	p->thread.eip = (unsigned long)ret_from_fork;
	//p->thread.esp = p->tss.esp0;
	p->thread.trace_bitmap =  0x80000000;

	childreg = (struct regs *)((p->thread.esp0) - (sizeof(struct regs)));

	pt_copy(childreg, (void*)(stack_start), sizeof(struct regs));

	p->thread.esp = (unsigned long)childreg;
	childreg->eax = 0;
	childreg->esp = (unsigned long)(user_stack[nr] + 64 * 4);

	task[nr] = p;

	//task_struct_dump(p);
	//kernel_stack_dump(childreg);
	printf("child_reg address : %08x\n", childreg);
	printf("thread.esp %08x\n", p->thread.esp);

	printf("task[%d] kernel mode sta top %08x\n",nr, p->thread.esp0);
	printf("task[%d] user mode stack top %08x\n",nr, childreg->esp);

	return last_pid;
}

int find_empty_process(void)
{
	/*int i = -1;

	printf("find_empty_process\n");

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
	printf("i = %d last_pid = %d\n", i,last_pid);

	return i;*/

	int i;

	i = ++last_pid;

	return i;
}
