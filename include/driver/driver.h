#ifndef __DRIVERS_H__
#define __DRIVERS_H__

#include <device.h>

#define MAX_DRIVER_NR               8
#define MAX_NAME_NR                16

struct driver
{
	char driver_name[MAX_NAME_NR];
	int (*probe)(struct device *device);
	struct file_operation *f_ops;
	struct driver *next;
};
extern struct driver *drivers[MAX_DRIVER_NR];

extern int driver_init(void);
extern int driver_register ( const char *__drv_name, struct driver *driver );

#endif

