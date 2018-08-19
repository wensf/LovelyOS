#include <io.h>
#include <keyboard.h>
#include <system.h>
#include <printk.h>

extern void _keyboard_irq( int cpl, unsigned int esp );

int keyboard_int_cnt = 0;

void keyboard_irq( int cpl, unsigned int esp )
{
	int r = inb(0x60);
	printk("key %02x\n",r);
}

void init_keyboard(void)
{
	/**
	 * Init the keyboard.
     */
    set_igate_descriptor(0x21, 3, _keyboard_irq);

	outb(inb(0x21) & ~0x2, 0x21);
}
