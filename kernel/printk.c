#include <stdarg.h>
#include <libc.h>
#include <init.h>
#include <printk.h>

extern void putchar( int x, int y, char ch, int color, int b_color);
static int x = 0, y = 0;

int printk(const char *fmt,...)
{
	char buf[256];
	va_list ap;
	
	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

    int i = 0;
	
#if 1

    while( buf[i] != '\0' )
	{
		if ( buf[i] == '\n'){
		    y += 16; x = 0;
			y %= kparam->yres;
		}else if ( buf[i] == '\t' ){
			x += 4*8;
			x %= kparam->xres;
			y = 0;
		}else if ( buf[i] == '\b'){
			if ( x >= 8 ) x -= 8;
			putchar(x,y,' ',CCOF, CCOB);
    	}else{
	    	putchar(x, y, buf[i], CCOF, CCOB);
			x += 8;
			if ( x >= kparam->xres )
			{
				x = 0; 
				y += 16;
				y %= kparam->yres;
			}
		}
		i++;
	}
#else
	// putchar(0, 0, 'K', 0xFF0000, 0);
	const int color[4] = {0xFF0000,0xFF00,0xFF,0xFFFFFF};
	for ( int i = 0; i < kparam->xres/2; i++ ){
		for ( int j = 0; j < 4; j++ ){
			setpixel(i,j,color[j]);
		}
	}
#endif
	
	return (i);
}



