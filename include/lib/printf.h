/**
 * File : os/include/lib/printf.h
 * header file for os/lib/printf.c
 * format print for operation system kernel.
 * 2016.09.08
 * last modified 2018.07.24
 */
#ifndef __PRINTF_H__
#define __PRINTF_H__

extern unsigned long vaddr;

extern int console_init(void);
extern int draw_text( int x, int y, const char *fmt,... );
extern int draw_text_kernel( int, int y, const char *fmt,... );
extern int printf(const char *fmt,...);

#endif // __PRINTF_H__
