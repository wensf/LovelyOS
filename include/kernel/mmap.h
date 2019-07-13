
/**
 * File kernel/mmap.h
 * Author : Wen Shifang
 * 2019.07.13
 */

#ifndef __MMAP_H__
#define __MMAP_H__

extern unsigned char *mmap(unsigned long start, unsigned long length, unsigned long flags);
extern unsigned char *syscall_mmap(unsigned long start, unsigned long length, unsigned long flags);

#endif





