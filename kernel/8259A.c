/**
 * 8259A.c
 * 8259A Programmer Interruption Controller(PIC).
 * 2016.6.21
 */
#include <io.h>
#include <8259A.h>

void init_8259A( void )
{
    outb(0x11,0x20);
	io_delay();
	outb(0x11,0xA0);
	io_delay();

	outb(0x20, 0x21);
	io_delay();
	outb(0x28, 0xA1);
	io_delay();

	outb(0x04, 0x21);
	io_delay();
	outb(0x02, 0xA1);
	io_delay();

	outb(0x01, 0x21);
	io_delay();
	outb(0x01, 0xA1);
	io_delay();

	outb(0xFE, 0x21);
	io_delay();
	outb(0xFF, 0xA1);
	io_delay();
}
