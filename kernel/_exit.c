#include <syscall.h>
#include <types.h>

void _exit( int exit_code )
{
never_return:
	__asm__("movl %1,%%ebx\n\t"
		"int $0x80"
		: /* no outputs */
		:"a" (_SYSCALL_exit),"g" (exit_code));
	goto never_return;
}


