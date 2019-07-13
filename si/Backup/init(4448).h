#ifndef __INIT_H__
#define __INIT_H__

struct kernel_param
{
	int xres;
	int yres;
	int bpp;
	int vaddr;
	int mem_cnt;
};

extern struct kernel_param *kparam;

#endif
