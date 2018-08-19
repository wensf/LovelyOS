/**
 * File : bus.c
 * created : 2018.8.4
 * last modified : 2018.8.4 
 */
#include <libc.h>
#include <bus.h>
#include <libc.h>
#include <device.h>
#include <driver.h>

struct bus_type *k_bus[MAX_BUS_TYPE];

int bus_init(void)
{
	memset(k_bus,0,sizeof(k_bus));
	
	return 0;
}

int bus_register(struct bus_type *bus)
{
	return 0;
}

int bus_add_device( struct device *device )
{
	struct device *p;
		
	p = device->bus->dev_root;
	
	while ( p != 0 )
	{
		if ( strcmp(p->device_name, device->device_name) )
		{
			// Error : Device Already Registered 
			return (-1);
		}
		p++;
	}	
		
	/* driver has no registerd, register now */
	struct device *r, *q;
	
	r = q = device->bus->dev_root;
	
	while ( q != 0 )
	{
		r = q;
		q++;
	}
	
	r->next = device;
	device->next = 0;
	
	/* try to match driver for device */
	
	return  device->bus->device_match_driver(device);
}

int bus_add_driver( struct driver *driver )
{
	struct driver *p;
		
	p = driver->bus->drv_root;
	
	while ( p != 0 )
	{
		if ( strcmp(p->driver_name, driver->driver_name) )
		{
			// Error : Driver Already Registered 
			return (-1);
		}
		p++;
	}	
		
	/* driver has no registerd, register now */
	struct driver *r, *q;
	
	r = q = driver->bus->drv_root;
	
	while ( q != 0 )
	{
		r = q;
		q++;
	}
	
	r->next = driver;
	driver->next = 0;
	
	/* try to match for device and driver */
	
	return  driver->bus->driver_match_device(driver);
}


