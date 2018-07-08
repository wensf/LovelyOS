/**
 * io.h the file provide the base Input/Output operation interface
 * 2016.6.20
 */
void outb(unsigned char value, unsigned short port)
{
	asm volatile("outb %%al, %%dx\n\t"
		::"a"(value),"d"(port)
	);
}

void outw(unsigned short value, unsigned short port)
{
	asm volatile(
		"outw %%ax, %%dx\n\t"
		::"d"(port),"a"(value)
	);
}

void outl(unsigned long value, unsigned long port)
{
	asm volatile("outl %%eax, %%dx\n\t"
		::"d"(port),"a"(value)
	);
}

unsigned char inb( unsigned short port)
{
	unsigned char value;

	asm volatile("inb %w1, %0\n\t"
		:"=a"(value)
		:"d"(port)
	);

	return value;
}

unsigned short inw( unsigned short port)
{
	unsigned short value;

	asm volatile ("inw  %w1, %0\n\t"
		:"=a"(value)
		:"d"(port)
	);

	return value;
}

unsigned long inl( unsigned long port)
{
	unsigned long value;

	asm volatile("inl  %w1, %0\n\t"
		:"=a"(value)
		:"d"(port)
	);

	return value;
}
