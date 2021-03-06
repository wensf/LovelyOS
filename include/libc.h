/**
 * File : PureRTOS/include/libc.h
 * Description : header file for PureRTOS/lib/libc.c
 * Author : Wen Shifang
 * Date : 2015.9.2
 */
#ifndef __LIBC_H__
#define __LIBC_H__

#include <stdarg.h>


#define MAX_BUF_LEN   1024


extern int vsnprintf(char *buf, int buf_len, const char *fmt, va_list ap);
extern int sprintf(char *buf, const char *fmt,...);
extern int strlen(const char *s);
extern int strcmp(const char *str1, const char *str2);
extern int strncmp(const char *str1, const char *str2, int len);
extern void memset(void *dst, char val, int size);
extern int strncpy(char *dst,const char*src, int size);
extern void memcpy(void *dst,const void *src, int size);
extern void strcpy(char *dst,const char *src);

#endif
