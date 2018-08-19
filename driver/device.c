#include <vfs.h>
#include <libc.h>
#include <bus.h>
#include <driver.h>
#include <device.h>

struct device *devices[MAX_DEVICE_NR];

int device_init(void)
{
	memset(devices, 0, sizeof(devices));
	
	return 0;
}

int device_register ( const char *__dev_name, struct device *device )
{
	struct device *p = 0;
	
	for ( int i = 0; i < MAX_DEVICE_NR; i++ )
	{
		if ( !strlen(devices[i]->device_name) )
		{
			p = devices[i];
			break;
		}
	}
	
	if ( p )
	{	
		/* Error : Device Already registered */
		return (-1);
	}
	
	for ( int i = 0; i < MAX_DEVICE_NR; i++ )
	{
		p = devices[i];
		if ( !p )
		{
			break;
		}
	}
	
	p = device;
	strcpy( p->device_name, __dev_name );
	
	return bus_add_device ( p );
}
