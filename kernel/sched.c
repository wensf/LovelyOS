#include <io.h>
#include <sched.h>
#include <system.h>
#include <libc.h>
#include <buffer.h>
#include <printk.h>

#if 1
#define _local_irq_save(x) 	\
__asm__ ("pushfl \n\t" 		\
	"popl %0 \n\t" 			\
	"cli" 					\
	:"=g" (x) 				\
	:						\
	:"memory")

#define _local_irq_restore(x)  	\
__asm__ ("pushl %0 \n\t" 		\
	"popfl" 					\
	:							\
	:"g" (x) 					\
	:"memory")
#else
#define _local_irq_save(x)
#define _local_irq_restore(x)
#endif

struct task_struct *task[TASK_NR];
static struct tss_struct tss;
struct task_struct *current;

extern void timer_int(void);

void schedule(void);

int timer_c = 0;
void timer_interrupt(int cpl, unsigned long sp )
{
    timer_c++;

    for ( int i = 0; i < sizeof(task)/sizeof(task[0]); i++ )
    {
		if ( !task[i] ) continue;
        if ( task[i]->delay > 0 )
        {
            task[i]->delay -= 10;
            if ( task[i]->delay <= 0 )
            {
                task[i]->state = TASK_RUNNING;
            }
        }
    }

    if ( cpl > 0 )
    {
		current->u_time++;
        schedule();
    }else{
		current->k_time++;
	}
}

void sched_init(void)
{
	/**
     * Make first task by manual
     */
	task[0] = (struct task_struct *)&buff[0];

	current = task[0];

	task[0]->pid      = 0x0;
	task[0]->state    = TASK_RUNNING;
	task[0]->delay    = 0;
	task[0]->u_time   = 0;
	task[0]->k_time   = 0;
	task[0]->prev     = 0x0;
	task[0]->next     = 0x0;
	task[0]->thread.ss0  = SELECTOR_KERNEL_DATA;
	task[0]->thread.esp0 = (unsigned long)(kernel_stack[0] + KERNEL_STACK_SIZE);
	task[0]->thread.ss   = SELECTOR_USER_DATA;
	task[0]->thread.esp  = (unsigned long)(user_stack[0] + USER_STACK_SIZE);

	/**
       * Initalize the TSS structure for CPU.
       * Each CPU has only one TSS structure
       */
	tss.back_link = 0;
	tss.ss0  = task[0]->thread.ss0;
	tss.esp0 = task[0]->thread.esp0;
	tss.ss   = task[0]->thread.ss;
	tss.esp  = task[0]->thread.esp;

	#if (SCHED_DEBUG_ENABLE==1)
	printk("struct tss size is %d byte(s), addr is %08x\n",
					sizeof(struct tss_struct),
					&tss
	);
	printk("tss esp0 : %08x\n", tss.esp0);
	printk("tss esp  : %08x\n", tss.esp);
	#endif

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
    outb(0x36, 0x43);
    outb( TIME_INTERVAL & 0xFF, 0x40);
    outb( TIME_INTERVAL>>8, 0x40);

    /**
     * enable the timer interrupt
     */
    outb(inb(0x21) & ~0x1, 0x21);
}

#if 0
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

#else
#define switch_to(prev, next, last)\
do{\
	__asm__ __volatile__(\
        "pushf\n\t"\
		"pushl %%ebp\n\t"\
		"movl %%esp, %0 \n\t" /* Save the ESP */\
		"movl %3, %%esp\n\t" /* ReStore ESP */\
		"movl $1f, %1\n\t" /* Save EIP */\
		"pushl %4\n\t" /* Restore EIP */\
		"ret\n\t"/*"jmp __switch_to\n\t"*/\
		"1:\n\t"\
		"popl %%ebp\n\t"\
		"popf\n\t"\
		:"=m"(prev->thread.esp), "=m"(prev->thread.eip),\
		 "=b"(last)\
		:"m"(next->thread.esp),"m"(next->thread.eip),\
		 "g"(prev),"d"(next),"c"(prev)\
	    :"memory"\
   );\
}while(0)
#endif


void task_struct_dump(struct task_struct *p);

void schedule(void)
{
	struct task_struct *next;
	struct task_struct *last;
	struct task_struct *prev;
	static int i = 1;
	unsigned long eflags;
	int nr, k;

	_local_irq_save(eflags);

	nr = sizeof(task)/sizeof(task[0]);
    k = 0;
#if 1
    do{
        i++;
        i %= nr;
        i = (i==0)?(1):(i);
        k++;
    }while ( (task[i]->state != TASK_RUNNING) && (k < nr) );
#endif // 0

    if ( k == nr )
    {
        i = 0;
    }

	if ( current ==  task[i] )
	{
		_local_irq_restore(eflags);
		return;
	}

 	next     = task[i];
	prev     = current;
	current  = next;

	tss.esp0 = next->thread.esp0;
	_local_irq_restore(eflags);
    switch_to(prev, next, last);
}
