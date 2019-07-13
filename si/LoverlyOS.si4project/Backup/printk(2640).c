#include <stdarg.h>
#include <libc.h>
#include <printk.h>

extern void putchar( int x, int y, char ch, int color, int b_color);
static int origin_x = 0, origin_y = 0;
static int x = 0, y = 0;

int printk(const char *fmt,...)
{
	char buf[512];
	va_list ap;
	
	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

    int i = 0;

	#if 0
	x = origin_x;
	y = origin_y;

    while( buf[i] != '\0' )
	{
		if ( buf[i] == '\n'){
		    y += 16; x = origin_x;
			if ( y > (360-16) )
			{
				y = origin_y = 0;
				origin_x += 1920/4;

				if ( origin_x > (1920-8) )
				{
					origin_x = 0;
				}

				x = origin_x;
			}
			origin_y = y;
		}else{
	    	putchar(x, y, buf[i], 0xFFFFFF, 0x80<<16);
			x += 8;
			if ( x > ( origin_x+(1920/4) ) )
			{
				x = origin_x;
			}
		}
		i++;
	}
	#endif

    while( buf[i] != '\0' )
	{
		if ( buf[i] == '\n'){
		    y += 16; x = 0;
			y %= 1080;
		}else if ( buf[i] == '\t' ){
			x += 4*8;
			x %= 1920;
			y = 0;
		}else if ( buf[i] == '\b'){
			if ( x >= 8 ) x -= 8;
			putchar(x,y,' ',0xFFFFFF, 0x80<<16);
    	}else{
	    	putchar(x, y, buf[i], 0xFFFFFF, 0x80<<16);
			x += 8;
			if ( x >= 1920 )
			{
				x = 0; 
				y += 16;
				y %= 1080;
			}
		}
		i++;
	}
	
	return (i);
}
