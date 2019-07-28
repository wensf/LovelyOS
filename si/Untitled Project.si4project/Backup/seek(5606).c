#include <sched.h>
#include <fs/vfs.h>
#include <fs/ramfs.h>

int syscall_lseek( int fd, int offset, int whence )
{
	return ramfs_lseek ( fd, offset, whence );
}

