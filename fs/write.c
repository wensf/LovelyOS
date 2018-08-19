#include <sched.h>
#include <vfs.h>

int syscall_write ( int fd, const char *__buf, int len )
{
	struct file *filp;
	int i;
	
	filp = current->file[fd];	

	i = filp_write( filp, __buf, len );
	
	return (i);
}
