#include <libc.h>
#include <syscall.h>
#include <stdio.h>

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


int fd_keyboard;


int gets(char *buf)
{
	int ch, i = 0;

	do{
		read(fd_keyboard,(char*)&ch, 1);
		if ( ch == '\n') break;
		printf(0,"%c",ch);
		buf[i++] = ch & 0xFF;
	}while (1);

	buf[i] = '\0';
	
	return (i);
}

int main(void)
{
 	int i = 0;
	int fd = 0;
	int fb;	

	fb = open("/dev/fb",O_RDWR,0);
	ioctl(fb,IOCTL_FB_INFO,(int)&fb_info);
	lseek(fd,fb_info.xres*16+0,SEEK_SET);
	
	printf(fd,"fb=%d,xres=%d, yres=%d\n", fb,fb_info.xres, fb_info.yres);

	fd_keyboard = open("/dev/keyboard", O_RD, NOBLK);
	
	lseek(fd,fb_info.xres*32+0,SEEK_SET);
	printf(fd,"Task sh is running, fb=%d\n", fb);
	
	lseek(fd,fb_info.xres*48+0,SEEK_SET);
	
	p_vram = (unsigned int*)mmap(fb,fb_info.xres*fb_info.yres*4,0);
	if ( p_vram ){
 		printf(fd,"mmap done p_vram=%08x\n", p_vram);
	}else{
		printf(fd,"mmap failed\n");
	}

	int x = fb_info.xres/2;
	int y = (fb_info.yres/3)*2;
	int color = 0;
	
	while(1)
	{
		lseek(fd,fb_info.xres*64+0,SEEK_SET);
		printf(fd,"Hello Loverly OS stack at %08x,i=%08d\n",(unsigned int)&i, i);
		i++;

		sleep(10000);

		draw_line(x,y,100,4,color);

		y += 8;
		y %= fb_info.yres;
		color += 10;

		if ( i > 30 ){
			exit(0);
		}
#if 1
		lseek(fd,fb_info.xres*80+0,SEEK_SET);
		char buf[32];
		while(1)
		{
			gets(buf);
			printf(fd, "\nLover#");
		}
#endif
	}
	
	return (0);
	
}









