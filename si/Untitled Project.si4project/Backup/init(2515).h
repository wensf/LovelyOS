#ifndef __INIT_H__
#define __INIT_H__


#define KERNEL_PAGE_DIR_ADDR	0x0100000			// 内核页目录地址(1MB)
#define VEDIO_VADDR             0x1000000			// 显存映射虚拟地址(16MB)

struct kernel_param
{
	int xres;
	int yres;
	int bpp;
	int vaddr;
	int vesa_mode;
	int mem_cnt;
};


extern struct kernel_param *kparam;

#endif
