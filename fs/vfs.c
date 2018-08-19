#include <vfs.h>
#include <libc.h>
#include <sched.h>
#include "chrdev.h"

struct struct_filesystem *file_systems[FILE_SYSTEM_NR];
int max_opened_file;

int vfs_init(void)
{
	memset(file_systems,0,sizeof(file_systems));	
	max_opened_file = FSAL_MAX_OPENED_FILE;
	
	return 0;
}

int fsal_filesystem_register( struct struct_filesystem  * filesystem )
{
	for ( int i = 0; i < FILE_SYSTEM_NR; i++ )
	{
		if ( !file_systems[i] )
		{
			file_systems[i] = filesystem;
			return (0);
		}
	}
	
	return (-1);
}

struct inode *inode_malloc(void)
{
	static struct inode inode_pool[8];
	static int i = 0;
	
	return &inode_pool[i++];
}

struct inode* filp_open( const char *__file_name, int mode, int flags )
{
	struct inode *inode;
		
	inode = inode_malloc();
	struct chrdrv *drv = lookup_driver(__file_name);
	
	if ( drv && drv->f_ops )
	{
		inode->private_data = drv->f_ops;
	}
	return inode;
}

int filp_read( struct file *filp, char * __buf, int len )
{
	if ( filp && filp->f_ops && filp->f_ops->f_open )
	{
		return filp->f_ops->f_read( filp, __buf, len );
	}
	
	return (-1);
}


int filp_write( struct file *filp, const char * __buf, int len )
{
	if ( filp && filp->f_ops && filp->f_ops->f_open )
	{
		return filp->f_ops->f_write( filp, __buf, len );
	}
	
	return (-1);
}

int filp_close( struct file *filp )
{
	if ( filp && filp->f_ops && filp->f_ops->f_close )
	{
		return filp->f_ops->f_close( filp );
	}
	
	return (-1); 
}

