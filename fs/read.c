#include <sched.h>
#include <fs/vfs.h>
#include <fs/ramfs.h>

int syscall_read( int fd, char *__buf, int len )
{
	return ramfs_read ( fd, __buf, len );
}

