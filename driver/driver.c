#include <vfs.h>
#include <libc.h>
#include <bus.h>
#include <device.h>
#include <driver.h>

struct driver *drivers[MAX_DRIVER_NR];

int driver_init(void)
{
	memset(drivers, 0, sizeof(drivers));
	
	return 0;
}

int driver_register ( const char *__drv_name, struct driver *driver )
{
	struct driver *p = 0;
	
	for ( int i = 0; i < MAX_DEVICE_NR; i++ )
	{
		if ( !strlen(drivers[i]->driver_name) )
		{
			p = drivers[i];
			break;
		}
	}
	
	if ( p )
	{	
		/* Error : Driver Already registered */
		return (-1);
	}
	
	for ( int i = 0; i < MAX_DRIVER_NR; i++ )
	{
		p = drivers[i];
		if ( !p )
		{
			break;
		}
	}
	
	p = driver;
	strcpy( p->driver_name, __drv_name );
	
	return bus_add_driver ( p );
}

