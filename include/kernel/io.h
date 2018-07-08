/**
 * io.h the file provide the base Input/Output operation interface
 * 2016.6.20
 */

#ifndef __IO_H__
#define __IO_H__

extern void outb(unsigned char value, unsigned short port);
extern void outw(unsigned short value, unsigned short port);
extern void outl(unsigned long value, unsigned long port);
extern unsigned char inb( unsigned short port);
extern unsigned short inw( unsigned short port);
extern unsigned long inl( unsigned long port);

#define io_delay() do\
{\
	asm volatile("nop");\
	asm volatile("nop");\
	asm volatile("nop");\
	asm volatile("nop");\
}while(0)

#endif
