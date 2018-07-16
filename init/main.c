/**
 * init/main.c
 * The main program entry.
 * Create :
 * Last modified : 2016.10.5
 */

#include <libc.h>
#include <traps.h>
#include <irq.h>
#include <8259A.h>
#include <sched.h>
#include <memory.h>
#include <system.h>
#include <version.h>
#include <printf.h>

void init(void);

int x = 10, y = 0x1;

int tty_set_location( int xx, int yy)
{
	if ( xx > 79 ){
		return -1;
	}

	if ( yy > 24 )
	{
		return -1;
	}

	x = xx; y = yy;

	return 0;
}

void Enum_pci_device( void );
void delay(void)
{
	volatile unsigned long c = 0;

	while( c < 65536 ){ c++; }
}

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

#define _SYSCALL_fork  0
#define _SYSCALL_pause 1
#define _SYSCALL_read  2
#define _SYSCALl_write 3

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

_syscall_0(int,fork)
_syscall_0(int,pause)
_syscall_3(int,read,int,fd,char *,buf,int,cnt)

void system_call_test(void)
{
	int fd = 0;
	char buf[4];
	int cnt = 4;

	read(fd, buf, cnt);
}

int last_pid;

void global_variable_init(void)
{
	last_pid = 0;
}

extern int timer_c;

int main( int argc, char *argv[] )
{
	int ret = 0;
	unsigned int ptr;

	global_variable_init();

	printf("A tiny operation system kernel for intel x86 cpu !\n");
	printf(version);
	printf("\n");

	trap_init();
	IRQ_init();
	init_8259A();
    mem_init(0x100000, 0x800000);

	ptr = get_free_page();
	if ( ptr != 0 )
	{
        printf("get_free_page address %08x\n", ptr);
	}else{
        printf("get_free_page failed\n");
	}

	free_page(ptr);

	ptr = get_free_page();
	if ( ptr != 0 )
	{
        printf("get_free_page address %08x\n", ptr);
	}else{
        printf("get_free_page failed\n");
	}

	sched_init();

	tty_set_location(10, 8);

    sti();
	move_to_user_mode();


	/**
	 * Execute a system int 0x80 to call 'fork'
	 * Inline Assembler syntax to avoid using
	 * stack in user mode.
	 */
	__asm __volatile__ ("mov $0, %eax");
	__asm __volatile__ ("int $0x80"
                     :"=a"(ret)
                     :);

	if( ret == 0 )
	{
		init();
	}

	int i = 10;
	for(;;)
	{
		tty_set_location(10,15);
		printf("In the parent timer_c = %d i = %d \n",timer_c, i);
		i++;
// 		pause();
// 		delay();
	}
}


void init(void)
{
	int i = 0;

	for(;;)
	{
		tty_set_location(10,16);
		printf("In the child timer_c = %d i = %d \n",timer_c, i);
		i++;
		i++;
		i++;
//		pause();
	}
}
