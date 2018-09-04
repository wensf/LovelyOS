#include <sched.h>
#include <fs/vfs.h>
#include <fs/ramfs.h>

int syscall_write ( int fd, const char *__buf, int len )
{
	return ramfs_write ( fd, __buf, len );
}
