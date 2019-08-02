#include <fs/vfs.h>
#include <tty.h>
#include <libc.h>
#include <chrdev.h>
#include <sched.h>
#include <printk.h>
#include <putc.h>
#include <init.h>

static int tty_open( struct file *filp );
static int tty_write ( struct file *filp, const char *__buf, int len );
static int tty_read( struct file *filp, char *__buf, int len );
static int tty_lseek( struct file *filp, int offset, int whence );
static int tty_close( struct file *filp );

struct file_operations tty_file_operation =
{
	.f_open  = tty_open,
	.f_write = tty_write,
	.f_read  = tty_read,
	.f_lseek = tty_lseek,
	.f_close = tty_close,
};

int tty_open( struct file *filp )
{		
	printk("task[%d] tty_open\n", current->pid);
	filp->w_o = kparam->xres * (kparam->yres/3);	
	
	return 0;
}

int tty_write( struct file *filp, const char *__buf, int len )
{
    int i = 0;

	int x, y;

	x = filp->w_o % kparam->xres;
	y = filp->w_o / kparam->yres;


	// printk("task[%d] tty_write len=%d, x=%d,y=%d\n", current->pid, len, x, y);	

    while( __buf[i] != '\0' )
	{
		if ( __buf[i] == '\n'){
			filp->w_o += ((kparam->xres-x)+kparam->xres-x/2);
		    y += 16; x = kparam->xres-x/2;
		}else{
	    	putchar(x, y, __buf[i], 0xFFFFFF, 0x101010);
			x += 8;
			filp->w_o += 8;
		}
		i++;
	}
	 
	if ( filp->w_o > kparam->xres * ((kparam->yres/3)*2) )
	{
		filp->w_o = kparam->xres * (kparam->yres/3);
	}
	
	return (i);
}

int tty_read( struct file *filp, char *__buf, int len )
{
	printk("task[%d] tty_read\n", current->pid);
	return 0;
}

int tty_lseek( struct file *filp, int offset, int whence )
{
	switch ( whence )
	{
	case SEEK_SET:
		filp->w_o = kparam->xres * (kparam->yres/3) + offset;
		break;
	case SEEK_CUR:
		filp->w_o += offset;
		break;
	case SEEK_END:
		filp->w_o = kparam->xres * kparam->yres - offset;
		break;			
	}
	return 0;
}

int tty_close( struct file *filp )
{
	printk("tty_close\n");
	return 0;
}

static struct chrdev tty_device[NR_TTY] =
{
	[0] = {

	},
	[1] = {

	},
	[2] = {

	},
	[3] =
	{

	},
};

static struct chrdrv tty_driver;

int tty_init(void)
{
	strcpy ( tty_driver.driver_name, "tty" );
	tty_driver.f_ops = &tty_file_operation;

	for ( int i = 0; i < NR_TTY; i++ )
	{
		sprintf ( tty_device[i].dev_name, "tty%d", i );
		chrdev_register( &tty_device[i], &tty_driver );
	}

	return 0;
}

