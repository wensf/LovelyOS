/**
 * File : os/lib/printf.c
 * format print for operation system kernel.
 * 2016.09.08
 */
#include <stdarg.h>
#include <libc.h>
#include <init.h>
#include <printf.h>
#include <font.h>
#include <sys_call.h>
#include <putc.h>

#define _syscall_3(type,name,atype,a,btype,b,ctype,c) \
type name(atype a, btype b, ctype c)\
{\
	long _res;\
	__asm__ __volatile__("int $0x80"\
			:"=a"(_res)\
			:"a"(_SYSCALL_write),"b"((long)(a)),\
			 "c"((long)(b)), "d"((long)(c))\
	);\
	if(_res >= 0)\
		return (type)(_res);\
	else{\
		return (type)(-1);\
	}\
}

inline _syscall_3(int,write,int,fd,char *,__buf, int, len)

int printf(const char *fmt,...)
{
	char buf[512];
	va_list ap;
	int i;

	memset(buf,0,sizeof(buf));

	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

	i = write( 0, buf, strlen(buf) );

	return (i);
}

int draw_text( int x, int y, const char *fmt,...)
{
	char buf[256];
	va_list ap;

	memset(buf,0,sizeof(buf));

	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

    int i = 0;

    while( buf[i] != '\0' )
	{
		if ( buf[i] == '\n'){
		    y += 16; x = 0;
		}else{
	    	putchar(x, y, buf[i], 0xFF00, 0x0);
			x += 8;
		}
		i++;
	}

	return 0;
}

unsigned long *vaddr;

int console_init(void)
{
	vaddr = (unsigned long *)(kparam->vaddr);

	return 0;
}
