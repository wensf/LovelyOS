#include <font.h>
#include <putc.h>

void setpixel( int x, int y, int color)
{
	if ( x > 1919 || y > 1079 )
	{
		return;
	}
	*((unsigned int *)(vaddr) + (1920*y + x)) = color;
}

void putchar( int x, int y, char ch, int color, int b_color)
{
	unsigned long temp;

	const unsigned char *bitmap = Font_8x16[ch-32];

	for ( int k = 0; k < 8; k++ )
	for ( int i = 0; i < 16/8; i++ )
	{
		temp = bitmap[(16/8)*k+i];

		for ( int j = 0; j < 8; j++ )
		{
			if ( (temp>>j) & 0x1)
			{
				setpixel(x+k, i*8+y+j, color);
			}else{
				setpixel(x+k, i*8+y+j, b_color);
			}
		}
	}
}
