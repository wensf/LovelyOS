#include <sched.h>
#include <vfs.h>

extern int file_free ( struct file* filp );

int syscall_close( int fd )
{
	struct file *filp;
	int i;
	
	if ( (fd < 0) && ( fd > (FSAL_MAX_OPENED_FILE-1)) )
	{
		return (-1);
	}
	
	if ( current->file_counter[fd] > 0 )
	{
		current->file_counter[fd]--;
	}
	if ( current->file_counter[fd] > 0 )
	{
		return 0;
	}
	
	filp = current->file[fd];	
	i = filp_close( filp );
	file_free(filp);
	
	return (i);
}

