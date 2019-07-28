#include <stdarg.h>
#include <libc.h>
#include <init.h>
#include <printk.h>

extern void putchar( int x, int y, char ch, int color, int b_color);
static int x = 0, y = 0;
static int ox, oy;

int b_color = CCOB, f_color = CCOF;

void set_color( int bcolor, int fcolor )
{
	b_color = bcolor;
	f_color = fcolor;
}


char buf[MAX_BUF_LEN];


int printk(const char *fmt,...)
{
	va_list args;

	memset(buf,0,sizeof(MAX_BUF_LEN));
	
	va_start(args, fmt);
	vsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);

    int i = 0;

	#if 0
    while( buf[i] != '\0' )
	{
		if ( buf[i] == '\n'){
		    y += 16; x = 0;
			y %= (kparam->yres-48);
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
				y %= (kparam->yres-48);
			}
		}
		i++;
	}
	#else

	x = ox, y = oy;

    while( buf[i] != '\0' )
	{
		if ( buf[i] == '\n'){
		    y += 16; x = ox;
			if ( y >= (kparam->yres-8) )
			{ 
				x += kparam->xres/3; 
				if ( x >= kparam->xres )
				{
					x = 0;
				}
			}
			y %= (kparam->yres);
		}else if ( buf[i] == '\t' ){
			x += 4*8;
			x %= kparam->xres;
			y = 0;
		}else if ( buf[i] == '\b'){
			if ( x >= 8 ) x -= 8;
			putchar(x,y,' ',f_color, b_color);
    	}else{
	    	putchar(x, y, buf[i], f_color, b_color);
			x += 8;
			if ( x >= kparam->xres )
			{
				x = 0; 
				y += 16;
				if ( y >= (kparam->yres-8) )
				{
					x += kparam->xres/3; 
					if ( x >= kparam->xres ){
						x = 0;
					}
				}
			}

			y %= (kparam->yres-8);
		}
		i++;
	}

	ox = x; oy = y;

	#endif
	
	return (i);
}



