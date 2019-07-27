#include <io.h>
#include <sched.h>
#include <system.h>
#include <init.h>
#include <libc.h>
#include <buffer.h>
#include <page.h>
#include <slab.h>
#include <printk.h>

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

struct task_struct *task[TASK_NR];
static struct tss_struct tss;
struct task_struct *current = 0;

extern void timer_int(void);

void schedule(void);

int draw_text( int x, int y, const char *fmt,...);

volatile int timer_c = 0;
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

int pid;
int state;
int delay;
int u_time;
int k_time;
struct thread_struct thread;
struct file *file[FSAL_MAX_OPENED_FILE];
int file_counter[FSAL_MAX_OPENED_FILE];
unsigned long signal;
int exit_code;
unsigned int pgd;
struct mm_struct mm;
int last_fd;
struct task_struct *parent;
struct task_struct *prev;
struct task_struct *next;


void task_dump(struct task_struct *task)
{
	printk("task [%d]----------sizoef(task[%d]) is %d bytes\n", task->pid, task->pid, sizeof(*task));
	printk("state=%08x\n",task->state);
	printk("delay=%08x\n",task->delay);
	printk("u_time=%08x\n",task->u_time);
	printk("k_time=%08x\n",task->k_time);
	printk("signal=%08x\n",task->signal);
	printk("exit_code=%08x\n",task->exit_code);
	printk("last_fd=%08x\n",task->last_fd);
	printk("pgd=0x%08x\n", task->pgd);
	printk("mm.stack_start=0x%08x\n",task->mm.stack_start);
}

#define TASK0_KERNEL_STACK(x) unsigned long __attribute__((aligned (PAGE_SIZE))) x[KERNEL_STACK_SIZE]
#define TASK0_USER_STACK(x) unsigned long __attribute__((aligned (PAGE_SIZE))) x[USER_STACK_SIZE]

TASK0_KERNEL_STACK(task_idle);
TASK0_USER_STACK(task_idle_user_stk);

void sched_init(void)
{
	/**
     * Make first task by manual
     */
	unsigned long struct_tcb = (unsigned long)task_idle;
	
	printk("struct_tcb[%d]=0x%08x\n", 0, struct_tcb);
	
	memset( task, 0, sizeof(task));

	/**
	 * Link the task
	 */
	for ( int i = 0; i < SIZEOF_NR(task)-1; i++ )
	{
		task[i]->next = task[i+1];
	}
	task[SIZEOF_NR(task)-1]->next = task[0];
	
	
	task[0] = (struct task_struct *)struct_tcb;

	current = task[0];

	task[0]->pid      = 0x0;
	task[0]->state    = TASK_RUNNING;
	task[0]->delay    = 0;
	task[0]->u_time   = 0;
	task[0]->k_time   = 0;
	task[0]->last_fd  = 0;
	for ( int i = 0; i < FSAL_MAX_OPENED_FILE; i++){
		task[0]->file[i] = 0;
	}	
	task[0]->signal = 0;
	task[0]->exit_code = 0;
	task[0]->thread.ss0  = SELECTOR_KERNEL_DATA;
	task[0]->thread.esp0 = struct_tcb + KERNEL_STACK_PAGES*PAGE_SIZE -4;
	task[0]->thread.ss   = SELECTOR_USER_DATA;
	struct vmb *vm = (struct vmb *)slab_alloc(sizeof(struct vmb));
	vm->vaddr = (uint32)task_idle_user_stk;//get_free_pages(USER_STACK_PAGES);
	vm->count = 0;
	vm->length = PAGE_SIZE;
	vm->next = 0;
	vm->attrs = P_P|P_US|P_RW;
	task[0]->mm.stack_start = vm;
	task[0]->thread.esp  = vm->vaddr + USER_STACK_SIZE - 4;
	task[0]->pgd = KERNEL_PAGE_DIR_ADDR; // kernel page directory initiazied at kernel.S setup_paging()
	task[0]->parent = 0;
	task[0]->prev = task[0];
	task[0]->next = task[0];

	printk("task[0] user stack at 0x%08x\n",task[0]->thread.esp);
	// task_dump(task[0]);

	/**
     * Initalize the TSS structure for CPU.
     * Each CPU has only one TSS structure
     */
	tss.back_link = 0;
	tss.ss0  = task[0]->thread.ss0;
	tss.esp0 = task[0]->thread.esp0;
	tss.ss   = task[0]->thread.ss;
	tss.esp  = task[0]->thread.esp;

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
     * Binary, mode 3, LSB/MSB, ch 0
     */
    outb(0x36, 0x43);
    outb( TIME_INTERVAL & 0xFF, 0x40);
    outb( TIME_INTERVAL>>8, 0x40);

    /**
     * Enable the timer interrupt
     */
    outb(inb(0x21) & ~0x1, 0x21);
}

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


/**
 * The simplest scheduling method, just for normal work well,
 * TASK_NR must be greater than or equal to 2 including idle tasks.
 */

void schedule(void)
{
	struct task_struct *next;
	struct task_struct *last;
	struct task_struct *prev;
	static int i = 0;
	unsigned long eflags;
	/* int nr, k; */

	_local_irq_save(eflags);

#if 0
	nr = sizeof(task)/sizeof(task[0]);
    k = 0;

    do{
        i++;
        i %= nr;
        i = (i==0)?(1):(i);
        k++;
    }while ( (!task[i] ) || ((task[i]->state != TASK_RUNNING) && (k < nr)) );

    if ( k == nr )
    {
        i = 0;
    }

#else

	do{
		i++; i %= 4;
	}while(!task[i] || task[i]->state != TASK_RUNNING);
#endif

#if 1
	if ( (current == task[i]) || !task[i] )
	{
		_local_irq_restore(eflags);
		return;
	}

 	next     = task[i];
	prev     = current;
	current  = next;
#endif

#if 1
	__asm__ __volatile__(\
			"movl %0,%%eax\n\t"\
			"movl %%eax, %%cr3\n\t"
			:\
			:"g"(current->pgd)\
			:"eax"\
			);
#endif
	
//	draw_text(1920/2,0,"-%d-",i);	
	tss.esp0 = next->thread.esp0;
	_local_irq_restore(eflags);
    switch_to(prev, next, last);
	
}



