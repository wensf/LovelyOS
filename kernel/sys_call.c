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
extern int _syscall_execve(const char *file_name);
extern int _syscall_get_pid( void );
extern int _syscall_mmap(unsigned long start, unsigned long length, unsigned long flags);
extern int _syscall_exit(int exit_code);
extern int _syscall_wait( int *exit_code );

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
	(fn_ptr)_syscall_get_pid,
	(fn_ptr)_syscall_mmap,
	(fn_ptr)_syscall_exit,
	(fn_ptr)_syscall_wait,	
};

const int nr_sys_calls = SIZEOF_NR(sys_call_table);

extern int timer_c;
extern int idle_cnt;

void syscall_pause(void)
{
	current->state = TASK_INTERRUPTABLE;
}

void syscall_sleep(int mseconds )
{
    current->delay = mseconds;
    current->state = TASK_SLEEP;
}

void syscall_idle(void)
{
	idle_cnt++;
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

unsigned int syscall_get_pid(void)
{
	return current->pid;
}

