#include <sched.h>
#include <vfs.h>
#include <namei.h>
#include <printk.h>

struct inode *do_pen ( const char *__file_name, int mode, int flags )
{
	struct inode * inode;

	const char *p, *q;

	if ( !__file_name )
	{
		return (0);
	}

	q = p = __file_name;
	repeat:
	while ( *p != '\0' && *p != '/' ) { p++; }

	if ( *p == '/' )
	{
		p++; q = p;
		if ( p == '\0' ) return 0;
		goto repeat;
	}
	inode = filp_open ( q, mode, flags );

	return inode;
}

struct file *file_malloc(void)
{
	static struct file filp_buf[8];
	static int i = 0;

	return &filp_buf[i++];
}

int file_free ( struct file* filp )
{
	return 0;
}

int get_fd(void)
{
	return 0;
}

#define IS_CHR(mode) (mode & 0x00000001)
#define IS_BLK(mode) (mode & 0x00000010)

int syscall_open( const char *__file_name, int mode, int flags )
{
	struct inode *inode;
	struct file *filp;
	
	if ( open_namei( __file_name, mode, flags, &inode ) != 0 )
	{
		return (-1);
	}

	#if 0
	int i_mode;
	i_mode = inode->i_mode;
	if ( IS_CHR(i_mode) ){
	
	}else if (IS_BLK(i_mode))
	{
		
	}else{
		return (-1);
	}
	#endif
	
	inode = do_pen(__file_name, mode, flags );
	if ( !inode )
	{
		return (-1);
	}

	filp = file_malloc();
	if ( !filp )
	{
	    printk("open %s failed at file_malloc()\n",__file_name);
		return (-1);
	}

	
	filp->f_ops = (struct file_operation*)inode->private_data;
	if ( !filp->f_ops )
	{
	    printk("open %s failed\n",__file_name);
		return (-1);
	}

	int fd = get_fd();
	current->file[fd] = filp;

	if ( filp && filp->f_ops && filp->f_ops->f_open )
	{
		return filp->f_ops->f_open(inode, filp);
	}

	return (-1);
}
