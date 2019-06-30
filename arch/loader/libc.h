/**
 * File : PureRTOS/include/libc.h
 * Description : header file for PureRTOS/lib/libc.c
 * Author : Wen Shifang
 * Date : 2015.9.2
 */
#ifndef __LIBC_H__
#define __LIBC_H__

#include <stdarg.h>

extern int vsnprintf(char *buf, int buf_len, const char *fmt, va_list ap);
extern int sprintf(char *buf, const char *fmt,...);
extern void memset(void *dst, char val, int size);
extern int strlen(const char *s);
extern int strncpy(char *dst,const char*src, int size);
extern void memcpy(void *dst,const void *src, int size);
extern void strcpy(char *dst,const char *src);
extern void trim( char *strIn, char *strOut );

#endif
