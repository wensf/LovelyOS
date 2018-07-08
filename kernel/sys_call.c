#include <system.h>
#include <libc.h>

extern int printf(const char *fmt, ...);
/**
 * Define at os/kernel/system_call.s
 */
extern void sys_call0_fork(void);
extern void sys_call1_pause(void);

void sys_call2_read(int fd, char *buf, int cnt)
{
	printf("system_call 1 : read()\n");
	printf("fd %d, buf 0x%08x cnt %d\n",fd,buf,cnt);
}

typedef void (*fn_ptr)(void);

const fn_ptr sys_call_table[]=
{
	(fn_ptr)sys_call0_fork,
	(fn_ptr)sys_call1_pause,
	(fn_ptr)sys_call2_read,
};

const int nr_sys_calls = sizeof(sys_call_table)/sizeof(sys_call_table[0]);
