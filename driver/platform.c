
#include <bus.h>
#include <libc.h>
#include <device.h>
#include <driver.h>
#include <platform.h>

int platform_bus_driver_match_device( struct driver *driver );
int platform_bus_device_match_driver( struct device *device );

struct bus_type bus_type_platform = 
{
	.device_match_driver = platform_bus_device_match_driver,
	.driver_match_device = platform_bus_driver_match_device,
};

int platform_bus_device_match_driver( struct device *device )
{
	struct device *driver;
	struct bus_type *bus;
	
	driver = device->bus->dev_root;
	bus = device->bus;

	while ( driver != 0 )
	{
		if ( !strcmp(driver->device_name, device->device_name) )
		{
			return bus->probe(device);
		}
		driver = driver->next;
	}
	
	return (-1);
}

int platform_bus_driver_match_device( struct driver * driver )
{
	struct device *device;
	struct bus_type *bus;
	
	device = driver->bus->dev_root;
	bus = device->bus;

	while ( device != 0 )
	{
		if ( !strcmp(device->device_name, driver->driver_name) )
		{
			return bus->probe(device);
		}
		device = device->next;
	}
	
	return (-1);
}

int platform_bus_probe( struct device *device )
{
	return device->driver->probe(device);
}

int platform_bus_init(void)
{
	bus_register(&bus_type_platform);
	
	return 0;
}