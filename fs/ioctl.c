#include <sched.h>
#include <fs/vfs.h>
#include <fs/ramfs.h>

int syscall_ioctl ( int fd, int cmd, int arg )
{
	return ramfs_ioctl ( fd, cmd, arg );
}

