#include <fs/vfs.h>
#include <fs/ramfs.h>
#include <fs/sysfs.h>
#include <libc.h>

struct struct_filesystem *file_systems[FILE_SYSTEM_NR];
int max_opened_file;

int vfs_init(void)
{
	memset(file_systems,0,sizeof(file_systems));	
	max_opened_file = FSAL_MAX_OPENED_FILE;
	
	ramfs_init();
	sysfs_init();
	
	return 0;
}

int vfs_filesystem_register( struct struct_filesystem  *filesystem )
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

int vfs_filesystem_mount( const char *mount_point, struct struct_filesystem  *filesystem)
{
	return 0;
}



