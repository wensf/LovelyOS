#include <sched.h>
#include <fs/vfs.h>
#include <fs/ramfs.h>
#include <printk.h>

int syscall_open( const char *__file_name, int mode, int flags )
{
	return ramfs_open(__file_name, mode, flags);
}

