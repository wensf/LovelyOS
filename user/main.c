#include <libc.h>
#include <syscall.h>

#define O_RD                    1
#define O_WR                    2
#define O_RDWR              O_RD|O_WR
#define O_CREAT                 4

#define SEEK_SET                0
#define SEEK_CUR                1
#define SEEK_END                2

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

void exit( int exit_code )
{
never_return:
	__asm__("movl %1,%%ebx\n\t"
		"int $0x80"
		: /* no outputs */
		:"a" (_SYSCALL_exit),"g" (exit_code));
	goto never_return;
}


int main(void)
{
 	int i = 0;
	
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
		lseek(0,1920*80,SEEK_SET);
		printf("Hello Loverly OS stack at %08x,i=%08d\n",(unsigned int)&i, i++);

		//if ( !(i % 3) ){
			sleep(2500);
		//}

		draw_line(x,y,100,4,color);

		y += 8;
		y %= 1080;
		color += 10;

		if ( i > 10 ){
			exit(-1);
		}
	}
	
	return (0);
	
}









