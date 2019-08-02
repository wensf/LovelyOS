#include <syscall.h>
#include <stdarg.h>
#include <libc.h>

int printf(int fd,const char *fmt,...)
{
	char buf[256];
	va_list ap;
	int i;

	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

	i = write( fd, buf, strlen(buf));

	return (i);
}


