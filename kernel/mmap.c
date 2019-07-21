#include <sched.h>
#include <types.h>
#include <kernel/mmap.h>
#include <printk.h>
#include <page.h>
#include <ramfs.h>
#include <init.h>

unsigned char *syscall_mmap(int fd, unsigned long size, unsigned long flags)
{
	return ramfs_mmap( fd, size, flags);
}


