#ifndef __BUS_H__
#define __BUS_H__

#define BUS_NAME_LEN  							16
#define MAX_BUS_TYPE             		 		 8

struct bus_type
{
	char bus_name[BUS_NAME_LEN];
	struct device *dev_root;		/* devices on the bus */
	struct driver *drv_root;        /* drivers on the bus */
	int (*device_match_driver)(struct device *device);
	int (*driver_match_device)(struct driver *driver);
	int (*probe)(struct device *device );
};

int bus_init(void);
int bus_register(struct bus_type *bus);
int bus_add_device( struct device *device );
int bus_add_driver( struct driver *driver );
int bus_match( struct device *device, struct driver *driver );

#endif

