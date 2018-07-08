/**
 * PureRTOS/lib/libc.c
 * some functions to deal with strings.
 * created : 2014.12.20
 * last modified : 2015.9.15
 */
__asm__(".code16gcc");
#include <stdarg.h>

#define MAX_BUF_LEN   64
#define ZEROPAD       0
#define SPACEPAD      1

int strlen(const char *s)
{
	const char *sc ;

	for(sc = s; *sc != '\0'; ++sc){
		;
	}

	return sc - s;
}

void strcpy(char *dst,const char *src)
{
	while(*src != '\0'){
		*dst++ = *src++;
	}

	*dst = '\0';
}
