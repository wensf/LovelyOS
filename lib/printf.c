/**
 * File : os/lib/printf.c
 * format print for operation system kernel.
 * 2016.09.08
 */
#include <stdarg.h>
#include <libc.h>
#include <printf.h>

extern int x, y;

extern void putc(int x, int y , char c);

int printf(const char *fmt,...)
{
	char buf[128] = {"hello"};
	va_list ap;

	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

    int i = 0;

    while( buf[i] != '\0' ){
		if ( buf[i] == '\n'){
		    y++; x = 10;
		}else
	    	putc(x++, y, buf[i]);

		i++;
	}

	return 0;
}
