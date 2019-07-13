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

int last_pid;

void global_variable_init(void)
{
	last_pid = 0;
}

extern volatile int timer_c;

const int color[] = {0x80<<16,0x80<<8,0x80<<0};

#include <graphics.h>
#include <font.h>

void vesa_test(void)
{
	unsigned int *p;

	for ( int y = 0; y < 360; y++ )
	{
		p = (unsigned int *)(kparam->vaddr) + 1920*y;

		for ( int i = 0; i < 1920; i++ )
		{
			*p++ = color[0];
		}
	}

	for ( int y = 360; y < 720; y++ )
	{
		p = (unsigned int *)(kparam->vaddr) + 1920*y;

		for ( int i = 0; i < 1920; i++ )
		{
			*p++ = color[1];
		}
	}

	for ( int y = 720; y < 1080; y++ )
	{
		p = (unsigned int *)(kparam->vaddr) + 1920*y;

		for ( int i = 0; i < 1920; i++ )
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
volatile uint32 idle_cnt = 0;

int main( int argc, char *argv[] )
{
	global_variable_init();
	kparam = (struct kernel_param *)(0x98000);
    mem_init(0x200000, 0x800000);
	page_init();
	vesa_test();
	console_init();
	printk("xres = % 4d, yres=% 4d, bpp=%d, video_addr=%08x\n",
					kparam->xres,
					kparam->yres,
					kparam->bpp,
					kparam->vaddr
	);

	struct mem_info_struct *mp;
	mp = (struct mem_info_struct *)(0x98000+sizeof(struct kernel_param));
	printk("BaseAddrL BaseAddrH LengthLow LengthHigh    Type\n");
	uint32 total_memory = 0;
	for ( int i = 0; i < kparam->mem_cnt; i++ )
    {
        printk("%08x  %08x  %08x  %08x  %08x\n",
               mp->address_low,
               mp->address_high,
               mp->length_low,
               mp->length_high,
               mp->type);
        if ( mp->type == 1)
        {
            total_memory += mp->length_low;
        }
        mp++;
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
	move_to_user_mode();
	if ( !fork() )
	{
		init();
	}
	for (;;) 
	{
		idle_cnt++;
		idle();
	}
}

void task_1(void);


void init(void)
{
    int pid, i = 0;
	int last_count = 0;

//	(void)open("/dev/tty1",O_RDWR, 0);
//	(void)dup(0);
//	(void)dup(0);

//	printf("test printf syscall\n");

	if (!(pid=fork()))
	{
		task_1();
	}

	for(;;)
	{
		i++;

		if ( last_count != timer_c )
		{
			last_count = timer_c;
			#if 0
			printf("pid=%d i=%08x,ticks: %08d utime=%8d ktime=%8d idle_cnt=%08d\n",
					current->pid,
					i,
					timer_c,
					get_utime(),
					get_ktime(),
					idle_cnt);
			lseek(0,0,SEEK_SET);
			#endif
		}

		// for ( int k = 0; k < 10000000; k++ );
		sleep(1000);
	}
}

void task_2(void);


void new_task_entry(void)
{
	printf("new task_entry,pid=%d\n", current->pid);

	task_2();
}


void task_1(void)
{
	int i = 0;
	int x = 0,y = 420+32;
	int last_count = 0;

//	int pid;

	draw_text(0,360+16,"stack addr=%08x", (unsigned long)&last_count);
#if 0
	if (!(pid=fork()))
	{
		// task_2();
		execve("/bin/sh");
		while(1);
	}
#endif

	for (;;)
	{
		i++;
		if ( last_count != timer_c )
		{
			last_count = timer_c;
			#if 0
			draw_text(0,360+16,"pid=%d i=%08x,ticks: %08d utime=%8d ktime=%8d",
					current->pid,
					i,
					timer_c,
					get_utime(),
					get_ktime()
			 );
			#endif
		}
#if 0

		 if( x > 0 )
		 #if
		 draw_v_line(x-1,y,1,96,0x80<<8);
		 draw_bitmap(x,y,79,96,gImage_girl);
		 #else
		 draw_v_line(x-1,y,  1,144,0x80<<8);
		 draw_bitmap(x,  y,120,144,(unsigned char*)(0x800000));
		 #endif
		 x++; x %= 1920;
#endif			 
		 sleep(2000);
#if 0
		float color_tmp = 0;

		float step = 256.0/360.0;
        for ( int y = 0; y < 360; y++ )
        {
            unsigned int *p;
            p = (unsigned int *)(0x200000) + 1920*y;
            int tc = (int)(((int)color_tmp) << ((i % 3)*8));

            for ( int x = 0; x < 1920; x++ )
            {
                *p++ = tc;
            }
            color_tmp += step;
        }

		memcpy( (unsigned int *)(kparam->vaddr) + 1920*720, (unsigned char *)(0x200000), 360 * 1920 * 4);
#endif // 0
	}
}

void task_3(void);

void task_2(void)
{
	int i = 0;
	int last_count = 0;

	int pid;

	if (!(pid=fork()))
	{
		task_3();
	}

	for (;;)
	{
		i++;
		if ( last_count != timer_c )
		{
			last_count = timer_c;
			draw_text(0,360+32,"pid=%d i=%08x,ticks: %08d utime=%8d ktime=%8d",
					current->pid,
					i,
					timer_c,
					get_utime(),
					get_ktime()
			 );
		}

		//for ( int k = 0; k < 100000; k++ );
		sleep(3000);
	}
}

void task_4(void);

void task_3(void)
{
	int i = 0, j = 0;
	int last_count = 0;

	int pid;

	if (!(pid=fork()))
	{
		task_4();
	}	
	
	for (;;)
	{
		i++;
		if ( last_count != timer_c )
		{
			last_count = timer_c;
			j++;
			draw_text(0,360+48,"pid=%d i=%08x,j=%08d, ticks: %08d utime=%8d ktime=%8d",
					current->pid,
					i,
					j,
					timer_c,
					get_utime(),
					get_ktime()
			 );
		}

		//for ( int k = 0; k < 5000000; k++ );
		
		sleep(4000);
	}
}

void task_4(void)
{
	int i = 0, j = 0;
	int last_count = 0;

	for (;;)
	{
		i++;
		if ( last_count != timer_c )
		{
			last_count = timer_c;
			j++;
			draw_text(0,360+64,"pid=%d i=%08x,j=%08d, ticks: %08d utime=%8d ktime=%8d",
					current->pid,
					i,
					j,
					timer_c,
					get_utime(),
					get_ktime()
			 );
		}
		
		sleep(5000);
	}
}
