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
extern int _syscall_lseek( int fd, int offset, int whence );
extern void _syscall_idle( void );
extern unsigned int _syscall_get_utime( void );
extern unsigned int _syscall_get_ktime( void );
extern unsigned int _syscall_execve(const char *file_name);

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
	(fn_ptr)_syscall_lseek,
	(fn_ptr)_syscall_idle,
	(fn_ptr)_syscall_get_utime,
	(fn_ptr)_syscall_get_ktime,
	(fn_ptr)_syscall_execve,
};

const int nr_sys_calls = sizeof(sys_call_table)/sizeof(sys_call_table[0]);

extern int timer_c;

void syscall_pause(void)
{
	current->state = TASK_INTERRUPTABLE;
}

void syscall_sleep(int mseconds )
{
    current->delay = mseconds;
    current->state = TASK_SLEEP;
}

int draw_text( int x, int y, const char *fmt,...);

unsigned int syscall_get_utime(void)
{
	return current->u_time;
}

unsigned int syscall_get_ktime(void)
{
	return current->k_time;
}


