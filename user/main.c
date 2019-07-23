#include <libc.h>
#include <syscall.h>

#define O_RD                    1
#define O_WR                    2
#define O_RDWR              O_RD|O_WR
#define O_CREAT                 4

#define NOBLK                 	1

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
static inline _syscall_3(int, ioctl, int, fd,unsigned long, cmd, unsigned long, arg)
static inline _syscall_3(int,read,int,fd,char *,__buf, int, len)


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


#define IOCTL_FB_INFO 0
struct fb_info_struct
{
	int xres;
	int yres;
};


struct fb_info_struct fb_info;



void draw_pixel( int x, int y, int color )
{
	if ( (x < fb_info.xres) && (y < (fb_info.yres)) )
	{
		*(p_vram+ y*fb_info.xres+x) = color;
	}
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
	int fd = 0;
	int fb;	
	int fd_keyboard;

	fb = open("/dev/fb",O_RDWR,0);
	ioctl(fb,IOCTL_FB_INFO,(int)&fb_info);
	lseek(fd,fb_info.xres*16+720,SEEK_SET);
	
	printf(fd,"xres=%d, yres=%d\n", fb_info.xres, fb_info.yres);
	
	lseek(fd,fb_info.xres*32+720,SEEK_SET);
	printf(fd,"Task sh is running, fb=%d\n", fb);
	
	lseek(fd,fb_info.xres*48+720,SEEK_SET);
	
	p_vram = (unsigned int*)mmap(fb,fb_info.xres*fb_info.yres*4,0);
	if ( p_vram ){
 		printf(fd,"mmap done p_vram=%08x\n", p_vram);
	}else{
		printf(fd,"mmap failed\n");
	}

	fd_keyboard = open("/dev/keyboard", O_RD, NOBLK);

	int x = fb_info.xres/2;
	int y = (fb_info.yres/3)*2;
	int color = 0;
	int key;
	
	while(1)
	{
		lseek(fd,fb_info.xres*64+720,SEEK_SET);
		printf(fd,"Hello Loverly OS stack at %08x,i=%08d\n",(unsigned int)&i, i);
		i++;

		if ( !(i % 3) ){
			sleep(1000);
		}

		draw_line(x,y,100,4,color);

		y += 8;
		y %= fb_info.yres;
		color += 10;

		if ( i > 30 ){
			exit(0);
		}
#if 0
		read(fd_keyboard,(char*)&key,1);
		printf(fd,"key=%d", key);
		switch ( key )
		{
		case 'J':
			break;
		case 'L':
			break;
		case 'I':
			break;
		case 'K':
			break;
		default:
			break;
		}
#endif
	}
	
	return (0);
	
}









