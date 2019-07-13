#include <sched.h>
#include <types.h>
#include <kernel/mmap.h>
#include <printk.h>
#include <page.h>
#include <init.h>

extern int __page_map ( uint32 pgd, uint32 pa, uint32 va, int attr );

unsigned char *do_mmap(unsigned long start, unsigned long length, unsigned long flags)
{
	printk("do_mmap() start %08x, length %08x, flags=%08x\n", start, length, flags);

	// 直接将显存地址返回到用户空间.
	// 显存地址 vram

	unsigned long p = 0x6000000;	// 96MB
	unsigned long vaddr = kparam->vaddr;	// 显存地址在kparam结构体中.

	printk("we map the video addr pa %08x to %08x\n", vaddr, p);

	for ( int i = 0; i < 0x800000/PAGE_SIZE; i++ ){
		__page_map(current->pgd, vaddr+i*PAGE_SIZE, p+i*PAGE_SIZE, P_P|P_US|P_RW);
	}
	
	return (p);
}

unsigned char *syscall_mmap(unsigned long start, unsigned long length, unsigned long flags)
{
	return do_mmap(start,length, flags);
}


