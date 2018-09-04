#ifndef __DEVICE_H__
#define __DEVICE_H__

#define MAX_DEVICE_NR               8
#define MAX_NAME_NR                16

struct device
{
	char device_name[MAX_NAME_NR];	
	struct driver *driver;
	int major, minor;
	struct device *next;
};

extern struct device *devices[MAX_DEVICE_NR];

extern int device_register ( const char *__dev_name, struct device *device );

#endif