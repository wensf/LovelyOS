#include <sched.h>
#include <fs/vfs.h>
#include <fs/ramfs.h>

extern int file_free ( struct file* filp );

int syscall_close( int fd )
{
	return ramfs_close ( fd );
}

