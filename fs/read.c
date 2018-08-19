#include <sched.h>
#include <fsal.h>

int syscall_read( int fd, char *__buf, int len )
{
	struct file *filp;
	int i;
	
	filp = current->file[fd];	

	i = filp_read( filp, __buf, len );
	
	return (i);
}

