#include <graphics.h>

extern void setpixel( int x, int y, int color);

void draw_bitmap(int fx, int fy, int width, int height, const unsigned char *bitmap )
{
	unsigned long rgb;
	const unsigned char *p = bitmap;

	for( int y = fy; y < (fy + height); y++ )
	{
		for (  int x = fx; x < ( fx + width); x++ )
		{
			rgb = ((unsigned long)p[2]<<16) | p[1]<<8 | p[0];
			p += 3;
			setpixel(x,y,rgb);
		}
	}
}

void draw_v_line( int fx, int fy, int w, int h, int color )
{
	for( int y = fy; y < (fy + h); y++ )
	{
		for (  int x = fx; x < ( fx + w); x++ )
		{
			setpixel(x,y,color);
		}
	}
}