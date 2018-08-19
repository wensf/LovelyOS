/**
 * File : os/arch/loader/printf.c
 * format a string to print.
 * Create        2016.09.08
 * Last modified 2018.07.18
 */
#include <stdarg.h>
#include <libc.h>
#include <console.h>

__asm__(".code16gcc");

int printf(const char *fmt,...)
{
	char buf[128];
	const char *p;
	#if 1
	va_list ap;

	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	#endif

    p = buf;

    while( *p != '\0' ){
		if ( *p == '\n'){
		    y++; x = 0;
		}else{
	    	putc(x++, y, *p);
		}

		p++;
	}

	return 0;
}
