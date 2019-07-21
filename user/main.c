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
static inline _syscall_3(unsigned char*, mmap, int, fd,unsigned long, size, unsigned long, flags)


int printf(int fd,const char *fmt,...)
{
	char buf[256];
	va_list ap;
	int i;

	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

//	i = write( 0, buf, strlen(buf) );
	i = write( fd, buf, strlen(buf));

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
	int fd;
	int fb;

	fd = open("/dev/tty0",O_RDWR, 0);
	(void)dup(0);
	(void)dup(0);	
	
	lseek(fd,1920*48+960,SEEK_SET);
	printf(fd,"Task sh is running\n");
	
	lseek(fd,1920*64+960,SEEK_SET);

	fb = open("/dev/fb",O_RDWR,0);
	// p_vram = (unsigned int*)mmap(fb,1920*1080*4,0);
	if ( p_vram ){
 		printf(fd,"mmap done p_vram=%08x\n", p_vram);
	}else{
		printf(fd,"mmap failed\n");
	}

	int x = 1920/2;
	int y = (1080/3)*2;
	int color = 0;
	
	while(1)
	{
		lseek(fd,1920*80+960,SEEK_SET);
		printf(fd,"Hello Loverly OS stack at %08x,i=%08d\n",(unsigned int)&i, i++);

		if ( !(i % 3) ){
			sleep(1000);
		}

		// draw_line(x,y,100,4,color);

		y += 8;
		y %= 1080;
		color += 10;

		if ( i > 10 ){
			exit(0);
		}
	}
	
	return (0);
	
}









