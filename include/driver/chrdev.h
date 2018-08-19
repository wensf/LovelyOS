#ifndef __CHRDEV_H__
#define __CHRDEV_H__

#define MAX_NAME_NR				16
#define NR_CHRDEV                8

struct chrdrv
{
	char driver_name[MAX_NAME_NR];
	struct file_operation *f_ops;	
};

struct chrdev
{
	char dev_name[MAX_NAME_NR];	
	struct chrdrv *driver;
};

extern struct chrdev *chrdevs[NR_CHRDEV];

extern int chrdev_init (void);
extern int chrdev_register( struct chrdev *chrdev, struct chrdrv *chrdrv );
extern struct chrdrv *lookup_driver( const char *dev_name );

#endif
