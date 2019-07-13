/**
 * init/main.c
 * The main program entry.
 * Create :
 * Last modified : 2016.10.5
 */
#include <init.h>
#include <libc.h>
#include <types.h>
#include <traps.h>
#include <irq.h>
#include <8259A.h>
#include <sched.h>
#include <memory.h>
#include <mm/page.h>
#include <system.h>
#include <version.h>
#include <printf.h>
#include <buffer.h>
#include <keyboard.h>
#include <printk.h>
#include <fs/vfs.h>
#include <chrdev.h>
#include <tty.h>
#include <printf.h>

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

#define _SYSCALL_fork  			0
#define _SYSCALL_pause 			1
#define _SYSCALL_sleep 			2
#define _SYSCALL_open  			3
#define _SYSCALL_read  			4
#define _SYSCALL_write 			5
#define _SYSCALL_close 			6
#define _SYSCALL_dup   			7
#define _SYSCALL_lseek      	8
#define _SYSCALL_idle       	9
#define _SYSCALL_get_utime     10
#define _SYSCALL_get_ktime     11
#define _SYSCALL_execve        12
#define _SYSCALL_getpid        13

#define _syscall_0(type,name) \
type name(void)\
{\
	long _res;\
	__asm__ __volatile__("int $0x80"\
			:"=a"(_res)\
			:"a"(_SYSCALL_##name)\
	);\
	if(_res >= 0)\
		return (type)(_res);\
	else{\
		return (type)(-1);\
	}\
}

#define _syscall_1(type,name,atype,a) \
type name(atype a)\
{\
	long _res;\
	__asm__ __volatile__("int $0x80"\
			:"=a"(_res)\
			:"a"(_SYSCALL_##name),"b"((long)(a))\
	);\
	if(_res >= 0)\
		return (type)(_res);\
	else{\
		return (type)(-1);\
	}\
}

#define _syscall_2(type,name,atype,a,btype,b) \
type name(atype a, btype b)\
{\
	long _res;\
	__asm__ __volatile__("int $0x80"\
			:"=a"(_res)\
			:"a"(_SYSCALL_##name),"b"((long)(a)),\
			 "b"((long)(b))\
	);\
	if(_res >= 0)\
		return (type)(_res);\
	else{\
		return (type)(-1);\
	}\
}

#define _syscall_3(type,name,atype,a,btype,b,ctype,c) \
type name(atype a, btype b, ctype c)\
{\
	long _res;\
	__asm__ __volatile__("int $0x80"\
			:"=a"(_res)\
			:"a"(_SYSCALL_##name),"b"((long)(a)),\
			 "c"((long)(b)), "d"((long)(c))\
	);\
	if(_res >= 0)\
		return (type)(_res);\
	else{\
		return (type)(-1);\
	}\
}

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

int last_pid;

extern volatile int timer_c;

const int color[] = {0x80<<16,0x80<<8,0x80<<0};

#include <graphics.h>
#include <font.h>

void vesa_test(void)
{
	unsigned int *p;

	int xres, yres;

	xres = kparam->xres;
	yres = kparam->yres;

	for ( int y = 0; y < (xres/3)*1; y++ )
	{
		p = (unsigned int *)(vaddr) + xres*y;

		for ( int i = 0; i < yres; i++ )
		{
			*p++ = color[0];
		}
	}

	for ( int y = (xres/3)*1; y < (xres/3)*2; y++ )
	{
		p = (unsigned int *)(vaddr) + xres*y;

		for ( int i = 0; i < yres; i++ )
		{
			*p++ = color[1];
		}
	}

	for ( int y = (xres/3)*2; y < (xres/3)*3; y++ )
	{
		p = (unsigned int *)(vaddr) + xres*y;

		for ( int i = 0; i < yres; i++ )
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

int idle_cnt;

int main( int argc, char *argv[] )
{
	kparam = (struct kernel_param *)(0x98000);
    mem_init(0x200000, 0x800000);
	console_init();
	page_init();
	//vesa_test();
	printk("xres = % 4d, yres=% 4d, bpp=%d,mode=%04x, video_addr=%08x\n",
					kparam->xres,
					kparam->yres,
					kparam->bpp,
					kparam->vesa_mode,
					kparam->vaddr
	);
	while(1);
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
	printk("%s\n",version);

	trap_init();
	IRQ_init();
	init_8259A();
	init_keyboard();
	vfs_init();
	chrdev_init();
	tty_init();
	sched_init();
    sti();
#if 0
	uint32 start = task[0]->mm.stack_start+USER_STACK_SIZE;
	__asm__ __volatile__("mov %0, %%eax\n\t"\
						 "mov %%eax, %%esp\n\t"::"c"(start));
#endif
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

void task_1(void);

void _exit(void){}

void init(void)
{
	int i = 0, last_count = 0;

	(void)open("/dev/tty1",O_RDWR, 0);
	(void)dup(0);
	(void)dup(0);

	printf("task_init test printf syscall stack=%08x\n", (uint32)&i);

#if 0

    int pid;

	if (!(pid=fork()))
	{
		execve("/bin/sh");
		_exit();
	} else	
#endif
	{
		for(;;)
		{
			i++;

			if ( last_count != timer_c )
			{
				last_count = timer_c;
				#if 1
				lseek(0,0,SEEK_SET);
				printf("pid=%d i=%08x,ticks: %08d utime=%8d ktime=%8d idle_cnt=%08d, st:%08x\n",
						getpid(),
						i,
						timer_c,
						get_utime(),
						get_ktime(),
						idle_cnt,
						(uint32)&i);
				#endif
			}
			sleep(1000);

		}
	}
}

#define DRAW_LOGO         0

void task_1(void)
 {
	int i = 0;
#if (DRAW_LOGO==1)
	int x = 0,y = 420+32+300;
#endif
	int last_count = 0;


	(void)open("/dev/tty1",O_RDWR, 0);
	(void)dup(0);
	(void)dup(0);

	printf("task_1 test printf syscall stack=%08x\n", (uint32)&i);

#if 1
	int pid;

	if (!(pid=fork()))
	{
		execve("/bin/sh");
	}else {
#endif

		for (;;)
		{
			i++;
			// if ( last_count != timer_c )
			{
				last_count = timer_c;

				#if 1
				lseek(0,1920*16,SEEK_SET);
				printf("pid=%d i=%08x,ticks: %08d utime=%8d ktime=%8d idle_cnt=%08d,st:%08x\n",
						current->pid,
						i,
						timer_c,
						get_utime(),
						get_ktime(),
						idle_cnt,
						(uint32)&i);
				#endif
				
				#if 0
				draw_text(0,360+32,"pid=%d i=%08x,ticks: %08d utime=%8d ktime=%8d",
						current->pid,
						i,
						timer_c,
						get_utime(),
						get_ktime()
				 );
				#endif
			}
#if (DRAW_LOGO==1)

			 if( x > 0 )
	 			draw_v_line(x-1,y,  1,144,0x80<<8);
	 		 draw_bitmap(x,  y,120,144,(unsigned char*)(0x800000));
	 		 x++; x %= 1920;
			 for ( int k =0; k < 100000; k++);;
#endif			 
			 sleep(2000);

		}
	}
}





