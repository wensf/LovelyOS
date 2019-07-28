
#include <vfs.h>
#include <ramfs.h>
#include <printk.h>
#include <page.h>
#include <init.h>
#include <sched.h>

int fb_open( struct file *filp );
int fb_read( struct file *filp, char *__buf, int len );
int fb_lseek( struct file *filp, int offset, int whence );
int fb_write( struct file *filp, const char *__buf, int len );
unsigned char *fb_mmap(struct file *filp, int size, int flags);
int fb_ioctl(struct file *filp, int cmd, int arg);
int fb_close( struct file *filp );


struct file_operations fb_fops = 
{
	.f_open  = fb_open,
	.f_read  = fb_read,
	.f_lseek = fb_lseek,
	.f_write = fb_write,
	.f_mmap  = fb_mmap,
	.f_ioctl = fb_ioctl,
	.f_close = fb_close,
};

int fb_open( struct file *filp )
{
	return (0);
}
int fb_read( struct file *filp, char *__buf, int len )
{
	return 0;
}
int fb_lseek( struct file *filp, int offset, int whence )
{
	return 0;
}
int fb_write( struct file *filp, const char *__buf, int len )
{
	return 0;
}

unsigned char *fb_mmap(struct file *filp, int size, int flags)
{	
	printk("do_mmap() size %08x, flags=%08x\n", size, flags);

	// 直接将显存地址返回到用户空间.
	// 显存地址 vram

	unsigned long p = 0x6000000;	// 96MB
	unsigned long vaddr = kparam->vaddr;	// 显存地址在kparam结构体中.

	printk("we map the video addr pa %08x to %08x\n", vaddr, p);

	for ( int i = 0; i < 0x800000/PAGE_SIZE; i++ ){
		page_map(current->pg_dir, vaddr+i*PAGE_SIZE, p+i*PAGE_SIZE,PAGE_SIZE, P_P|P_US|P_RW);
	}
	
	return (unsigned char *)(p);
}


struct fb_info_struct
{
	int xres;
	int yres;
};


int fb_ioctl(struct file *filp, int cmd, int arg)
{	
	printk("task[%d] fb_ioctl cmd=0x%08x, arg=0x%08x\n", current->pid, cmd, arg);

	switch ( cmd )
	{
	case 0:
		((struct fb_info_struct*)arg)->xres = kparam->xres;
		((struct fb_info_struct*)arg)->yres = kparam->yres;
		break;

	}

	return (0);
}


int fb_close( struct file *filp )
{
	return 0;
}


int fb_init(void)
{
	return ramfs_create_file ( "/dev/fb", &fb_fops, FILE );
}



