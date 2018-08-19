#include <system.h>
#include <libc.h>
#include <printk.h>
#include <sched.h>
#include <sys_call.h>

/**
 * Define at os/kernel/system_call.s
 */
extern void _syscall_fork(void);
extern void _syscall_pause(void);
extern void _syscall_sleep( int mseconds );
extern int _syscall_open( const char *file_name, int mode, int flag );
extern int _syscall_read( int fd, char *__buf, int cnt );
extern int _syscall_write( int fd, const char *__buf, int cnt );
extern int _syscall_close( int fd );
extern int _syscall_dup( int fd );

typedef void (*fn_ptr)(void);

const fn_ptr sys_call_table[]=
{
	(fn_ptr)_syscall_fork,
	(fn_ptr)_syscall_pause,
	(fn_ptr)_syscall_sleep,
	(fn_ptr)_syscall_open,
	(fn_ptr)_syscall_read,
	(fn_ptr)_syscall_write,
	(fn_ptr)_syscall_close,
	(fn_ptr)_syscall_dup,	
};

const int nr_sys_calls = sizeof(sys_call_table)/sizeof(sys_call_table[0]);

extern int timer_c;

void syscall_pause(void)
{
    //int c = 0;

    //while(c++<100)
    {
    //    printk("system call pause current_task id=%d %d\n", current->pid,timer_c);
    }
    // current->state = TASK_INTERRUPTABLE;
	current->state = TASK_RUNNING;
}

void syscall_sleep(int mseconds )
{
    current->delay = mseconds;
    current->state = TASK_SLEEP;
}


