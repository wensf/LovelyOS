#include <libc.h>
#include <chrdev.h>
#include <fs/ramfs.h>

struct chrdev *chrdevs[NR_CHRDEV];

int add_device_node ( const char *dev_name, struct file_operations *f_ops, int mode )
{
	char buf[32];
	
	sprintf ( buf, "/dev/%s", dev_name );
	
	ramfs_create_file ( buf, f_ops, FILE );
	
	return 0;
}

int chrdev_register( struct chrdev *chrdev, struct chrdrv *chrdrv )
{
	int i = 0;
	
	for ( ; i < NR_CHRDEV; i++ )
	{
		if ( !chrdevs[i] )
		{
			break;
		}
	}
	
	if ( i > (NR_CHRDEV-1) )
	{
		return (-1);
	}
	
	chrdev->driver = chrdrv;
	chrdevs[i] = chrdev;
	
	add_device_node(chrdev->dev_name, chrdrv->f_ops, 0);
	
	return 0;
}

struct chrdrv *lookup_driver( const char *dev_name )
{
	struct chrdrv *drv = 0;
	
	for ( int i = 0; i < NR_CHRDEV; i++ )
	{
		if ( !strcmp( chrdevs[i]->dev_name, dev_name ) )
		{
			drv = chrdevs[i]->driver;
			break;
		}
	}
	
	return drv;
}

int chrdev_init (void)
{
	memset ( chrdevs, 0, sizeof(chrdevs) );
	
	return 0;
}