#include <io.h>
#include <sched.h>
#include <system.h>
#include <libc.h>
#include <buffer.h>

struct task_struct *task[TASK_NR];
static struct tss_struct tss;
struct task_struct *current;

void printf(const char *fmt,...);

extern void timer_int(void);

void schedule(void);

int timer_c = 0;
void timer_interrupt(void)
{
    timer_c++;

    if ( timer_c > 100 )
    {
        schedule();
    }
}

void sched_init(void)
{
	task[0] = (struct task_struct *)&buff[0];

	current = task[0];

	task[0]->pid      = 0x0;
	task[0]->state    = TASK_INTERRUPTABLE;
	task[0]->prev     = 0x0;
	task[0]->next     = 0x0;
	task[0]->thread.ss0  = SELECTOR_KERNEL_DATA;
	task[0]->thread.esp0 = (unsigned long)(kernel_stack[0] + 1024*4);
	task[0]->thread.ss   = SELECTOR_USER_DATA;
	task[0]->thread.esp  = (unsigned long)(user_stack[0] + 1024*4);

	tss.back_link = 0;
	tss.ss0  = task[0]->thread.ss0;
	tss.esp0 = task[0]->thread.esp0;
	tss.ss   = task[0]->thread.ss;
	tss.esp  = task[0]->thread.esp;

	printf("struct tss size is %d byte(s), addr is %08x\n",
					sizeof(struct tss_struct),
					&tss
	);
	printf("tss esp0 : %08x\n", tss.esp0);
	printf("tss esp  : %08x\n", tss.esp);

	set_tss_descriptor(5, tss);
	__asm__ __volatile__(\
		"mov %0, %%ax\n\t"\
		"ltr %%ax\n\t"\
		::"i"(TSS_SEGMENT)\
	);

	/**
	 * Init the 8253 Timer.
     */
    set_igate_descriptor(0x20, 3, timer_int);

    /**
     * binary, mode 3, LSB/MSB, ch 0
     */
    #define HZ 100
    #define val (1193180/HZ)
    outb(0x36, 0x43);
    outb( val & 0xFF, 0x40);
    outb( val>>8, 0x40);

    /**
     * enable the timer interrupt
     */
    outb(inb(0x21) & ~0x1, 0x21);
}

#define __switch_to(prev, next, last)\
do{\
	__asm__ __volatile__("movl %0, %%eax\n\t"\
		"movl %1, %%edx\n\t"\
		::"m"(prev),"m"(next))\
}while(0)

#define switch_to(prev, next, last)\
do{\
	__asm__ __volatile__(\
        "pushl %%esi\n\t"\
		"pushl %%edi\n\t"\
		"pushl %%ebp\n\t"\
		"movl %%esp, %0 \n\t" /* Save the ESP */\
		"movl %3, %%esp\n\t" /* ReStore ESP */\
		"movl $1f, %1\n\t" /* Save EIP */\
		"pushl %4\n\t" /* Restore EIP */\
		"ret\n\t"/*"jmp __switch_to\n\t"*/\
		"1:\n\t"\
		"popl %%ebp\n\t"\
		"popl %%edi\n\t"\
		"popl %%esi\n\t"\
		:"=m"(prev->thread.esp), "=m"(prev->thread.eip),\
		 "=b"(last)\
		:"m"(next->thread.esp),"m"(next->thread.eip),\
		 "g"(prev),"d"(next),"c"(prev)\
   );\
}while(0)

void schedule(void)
{
	struct task_struct *next;
	struct task_struct *last;
	struct task_struct *prev;
	static int id = 1;

	next = task[id];
	prev = current;
	current = next;

	if ( id == 0 ){
		id = 1;
	}else{
		id = 0;
	}

	tss.esp0 = next->thread.esp0;
	switch_to(prev, next, last);
}
