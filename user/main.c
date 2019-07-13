#include <libc.h>

#define O_RD                    1
#define O_WR                    2
#define O_RDWR              O_RD|O_WR
#define O_CREAT                 4

#define SEEK_SET                0
#define SEEK_CUR                1
#define SEEK_END                2

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
#define _SYSCALL_mmap          14

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
static inline _syscall_3(int,write,int,fd,char *,__buf, int, len)
static inline _syscall_3(int,lseek,int, fd,int,offset, int, whence)
static inline _syscall_0(void,idle)
static inline _syscall_0(unsigned int, get_utime)
static inline _syscall_0(unsigned int, get_ktime)
static inline _syscall_1(unsigned int, execve, const char *, file_name)
static inline _syscall_3(unsigned char*, mmap, unsigned long, start,unsigned long, length, unsigned long, flags)


int printf(const char *fmt,...)
{
	char buf[256];
	va_list ap;
	int i;

	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

	i = write( 0, buf, strlen(buf) );

	return (i);
}

unsigned int *p_vram;

inline void draw_pixel( int x, int y, int color )
{
	*(p_vram+ y*1920+x) = color;
}

void draw_line(int x, int y, int w, int h, int color)
{
	for ( int i = y; i < (y+h); i++ )
	{
		for ( int j = x; j < (x+w); j++ ){
			draw_pixel(j,i,color);
		}
	}
}

int main(void)
{
 	int i = 0;
	
	open("/dev/tty1",O_RDWR,4);
	lseek(0,1920*48,SEEK_SET);

	printf("Task sh is running\n");
	lseek(0,1920*64,SEEK_SET);
	
	p_vram = (unsigned int*)mmap(0,1920*1080*4,0);
	if ( p_vram ){
 		printf("mmap done p_vram=%08x\n", p_vram);
	}else{
		printf("mmap failed\n");
	}

	int x = 1920/2;
	int y = (1080/3)*2;
	int color = 0;
	
	while(1)
	{
		lseek(0,1920*32,SEEK_SET);
		printf("Hello Loverly OS stack at %08x,i=%08d\n",(unsigned int)&i, i++);

		if ( !(i % 3) ){
			sleep(10000);
		}

		// draw_line(x,y,100,4,color);

		y += 8;
		y %= 1080;
		color += 10;
	}
	
	return (0);
	
}









