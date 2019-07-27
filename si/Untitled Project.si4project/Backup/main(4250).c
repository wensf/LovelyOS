/**
 * init/main.c
 * The main program entry.
 * Create :
 * Last modified : 2016.10.5
 *  Copyright (C) 2016, 2016 Wen Shifang.
 */
#include <init.h>
#include <libc.h>
#include <types.h>
#include <traps.h>
#include <irq.h>
#include <8259A.h>
#include <sched.h>
#include <page.h>
#include <system.h>
#include <version.h>
#include <printf.h>
#include <buffer.h>
#include <keyboard.h>
#include <printk.h>
#include <fs/vfs.h>
#include <chrdev.h>
#include <tty.h>
#include <syscall.h>
#include <printf.h>
#include <_exit.h>
#include <slab.h>
#include <fb.h>

struct kernel_param *kparam;
/**
 * SS,ESP,EFLAGS,CS,EIP
 */
#define move_to_user_mode() \
	__asm__ __volatile__(\
	  "movl %%esp, %%eax\n\t" \
	  "pushl $0x23\n\t" \
	  "push %%eax\n\t" \
	  "pushfl\n\t" \
	  "pushl $0x1B\n\t" \
	  "pushl $1f\n\t" \
	  "iret\n\t" \
	  "1: movl $0x23,%%eax\n\t" \
	  "mov %%ax, %%ds\n\t" \
	  "mov %%ax, %%es\n\t" \
	  "mov %%ax, %%gs\n\t" \
	  "mov %%ax, %%fs\n\t" \
	  ::)

static inline _syscall_0(int,fork)
static inline _syscall_0(int,pause)
static inline _syscall_1(int,sleep,int,mseconds)
static inline _syscall_3(int,open,const char *,file_name, int, mode, int, flag)
static inline _syscall_1(int,dup,int,fd)
static inline _syscall_3(int,lseek,int, fd,int,offset, int, whence)
static inline _syscall_0(void,idle)
static inline _syscall_0(unsigned int, get_utime)
static inline _syscall_0(unsigned int, get_ktime)
static inline _syscall_1(int32, execve, const char *, file_name)
static inline _syscall_0(unsigned int, getpid)
static inline _syscall_1(int,close,int,fd)
static inline _syscall_1(int,wait,int *,pid)	

extern volatile int timer_c;

const int color[] = {0x101010,0x101010,0x101010};

#include <graphics.h>
#include <font.h>

void vesa_test(void)
{
	unsigned int *p;

	int xres, yres;

	xres = kparam->xres;
	yres = kparam->yres;

	for ( int y = 0; y < (yres/3)*1; y++ )
	{
		p = (unsigned int *)(vaddr) + xres*y;

		for ( int i = 0; i < xres; i++ )
		{
			*p++ = color[0];
		}
	}

	for ( int y = (yres/3)*1; y < (yres/3)*2; y++ )
	{
		p = (unsigned int *)(vaddr) + xres*y;

		for ( int i = 0; i < xres; i++ )
		{
			*p++ = color[1];
		}
	}

	for ( int y = (yres/3)*2; y < (yres/3)*3; y++ )
	{
		p = (unsigned int *)(vaddr) + xres*y;

		for ( int i = 0; i < xres; i++ )
		{
			*p++ = color[2];
		}
	}
}

struct mem_info_struct
{
    unsigned int address_low;
    unsigned int address_high;
    unsigned int length_low;
    unsigned int length_high;
    unsigned int type;
};

void init(void);
extern long __code_start,__data_start,__bss_start;
extern long __code_end, __data_end, __bss_end;
extern long __start, __end;

int idle_cnt;

int main( int argc, char *argv[] )
{
	kparam = (struct kernel_param *)(0x98000);
    mem_init(0x200000, 0x800000);
	console_init();
	page_init();
	slab_init();
	vesa_test();		
	printk("xres = % 4d, yres=% 4d, bpp=%d, mode=%04x, video_addr=%08x\n",
					kparam->xres,
					kparam->yres,
					kparam->bpp,
					kparam->vesa_mode,
					kparam->vaddr
	);
	printk("Kernel Info:\ncode_start 0x%08x, data_start 0x%08x, bss_start 0x%08x\n"
		"code end   0x%08x, data_end   0x%08x, bss_end   0x%08x\n",
		&__code_start,&__data_start,&__bss_start,
		&__code_end,&__data_end,&__bss_end);
	struct mem_info_struct *ms;
	ms = (struct mem_info_struct *)(0x98000+sizeof(struct kernel_param));
	printk("BaseAddrL BaseAddrH LengthLow LengthHigh    Type\n");
	uint32 total_memory = 0;
	for ( int i = 0; i < kparam->mem_cnt; i++ )
    {
    	#if 0
        printk("%08x  %08x  %08x  %08x  %08x\n",
               ms->address_low,
               ms->address_high,
               ms->length_low,
               ms->length_high,
               ms->type);
		#endif
        if ( ms->type == 1)
        {
            total_memory += ms->length_low;
        }
        ms++;
    }
    printk("RAM size %08x(%dKB)\n", total_memory, total_memory/1024);
	printk("low_memory_start = 0x%08x, low_memory_end=0x%08x\n",
		low_memory_start, low_memory_end);
	printk("%s\n",version);

	trap_init();
	IRQ_init();
	init_8259A();
	vfs_init();
	chrdev_init();
	tty_init();
	keyboard_init();
	fb_init();
	sched_init();
    sti();
	move_to_user_mode();
	if ( !fork() )
	{
		init();
	}
	for (;;) 
	{
		idle();
	}

}

int volatile s_seek(int fd, int offset, int wherence)
{
	lseek(fd,offset,wherence);

	return 0;
}

void init(void)
{
	int i = 0;

	(void)open("/dev/tty0",O_RDWR, 0);
	(void)dup(0);
	(void)dup(0);

	lseek(0,kparam->xres*0+120,SEEK_SET);
	printf("task init, stack_top=0x%08x\n", (uint32)&i);

    int pid;

	for (;;)
	{
	#if 1
		if ((pid=fork()) < 0)
		{
			printf("init: fork() failed\n\r");
			continue;
		}
		if (!pid) {
			close(0);close(1);close(2);
			(void)open("/dev/tty0",O_RDWR,0);
			(void)dup(0);
			(void)dup(0);
			_exit(execve("/bin/sh")); // exit if execve failed
		}
		while(1){
			if (pid==wait(&i)){
				break;
			}
		}
	
		lseek(0,kparam->xres*80+120,SEEK_SET);
		printf("\r\nchild 0x%x died with code %d\n\r", pid, i);
		sleep(3000);

	#else
		if ( fork() == 0 ){
			for ( int i = 0; i < 100; i++ ){
				st[i] = i;
				
 				lseek(0,1920*16+960,SEEK_SET);
 				printf("task[%d],st[%d]=%d,stack=0x%08x,p =%d\n",getpid(),i,st[i],&i,p);
				sleep(15000);
			}

			_exit(0);
		}else{

			for ( int i = 0; i < 100; i++ ){
				st[i] = i;
				p++;
				lseek(0,1920*32+960,SEEK_SET);
				
				printf("task[%d],st[%d]=%d,stack=0x%08x,p=%d\n",getpid(),i,st[i],&i,p);
				sleep(20000);
			}
			pid = wait(&i);
			printf("wait() %d,%d\n",pid, i);
			while(1);
		}

		while(1);
	#endif
	
	}
}







