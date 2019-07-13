#include <fs/ramfs.h>
#include <fs/vfs.h>
#include <fs/ram_file.h>
#include <types.h>
#include <sched.h>
#include <printk.h>

struct struct_filesystem ramfs = 
{
};

int split_path( const char *path, char *p_path_name )
{
	return 0;
}

int get_last_unused(void)
{
	int last_unused;

	last_unused = current->last_fd++;
	return last_unused;
}

int file_free( struct file *filp )
{
	return 0;
}

int ramfs_create_dir( const char *path, int attr)
{
	char path_name[MAX_NAME];
	int32 p_id;

	if ( !path ) return (-1);
	if ( split_path(path, path_name) ) return (-1);

	p_id = ramfile_fid_find((const int8*)path_name);
	if ( p_id != -1 ) return (0);

	if ( ramfile_create_dir(p_id,0,(const int8*)path) < 0 )
	{
		return (-1);
	}
	
	printk("dir %s created\n", path);
	
	return 0;
}

int ramfs_create_file( const char *path, struct file_operations *f_ops, int flags)
{
	char path_name[MAX_NAME];
	int32 p_id;

	if ( !path ) return (-1);
	if ( split_path(path, path_name) ) return (-1);

	p_id = ramfile_fid_find((const int8*)path_name);
	if ( p_id != -1 ) return (0);
	
	if ( !f_ops ) return (0);

	if ( ramfile_create_file(p_id,0,(const int8*)path,f_ops,flags) < 0 )
	{
		return (-1);
	}
	
	printk("file %s created\n", path);
	
	return 0;
}

struct file* file_alloc(void)
{
	static struct file file_table[8];
	static int file_last = 0;

	return &file_table[file_last++];
}

extern int file_free(struct file* filp);


int ramfs_open( const char *path, int mode, int flags )
{
	struct file *filp;
	int32 f_id;
	
	printk("in ramfs_open\n");
	
	f_id = ramfile_fid_find((const int8*)path);
	if ( (f_id < 0) && !(mode|O_CREAT) )
	{	
		printk("not found %s\n", path);
		return (-1);
	}
	
	#if 0
	if ( mode|O_CREAT )
	{
		/* open with O_CREATE
		 * Do nothing now */
		return (-1);
	}
	#endif
		
	ram_file *ram_filp;
	
	ram_filp = &ram_files[f_id];
	filp = file_alloc();
	if ( !filp )
	{
		printk("file_alloc failed\n");
		return (-1);
	}
	
	filp->f_ops = ram_filp->f_ops;
	filp->r_o = ram_filp->f_pos[0];
	filp->w_o = ram_filp->f_pos[1];
	
	int last_unused = get_last_unused();
	if ( last_unused == -1 )
	{
		file_free(filp);
		printk("last_unused failed\n");
		return (-1);
	}
	printk("ramfs_open fd=%d\n",last_unused);
	
	current->file[last_unused] = filp;
	current->file_counter[last_unused]++;
	
	if ( filp->f_ops && filp->f_ops->f_open )
 	{	
 		printk("to call f_open()\n");
		filp->f_ops->f_open(filp);
	}else{
		printk("no f_open to call\n");
	}
		
	return last_unused;
}

int ramfs_read( int fd, char *__buf, int len )
{
	struct file *filp;
	int i = 0;
	
	filp = current->file[fd];
	
	if ( filp && filp->f_ops && filp->f_ops->f_read )
	{
		i = filp->f_ops->f_open(filp);
	}
	
	return (i);
}

int ramfs_lseek( int fd, int offset, int whence )
{
	struct file *filp;
	int i = -1;
	
	filp = current->file[fd];
	
	if ( filp && filp->f_ops && filp->f_ops->f_lseek )
	{
		i = filp->f_ops->f_lseek(filp, offset, whence);
	}
	
	return (i);	
}

int ramfs_write( int fd, const char *__buf, int len )
{
	struct file *filp;
	int i = 0;
		
	filp = current->file[fd];	
	
	if ( filp && filp->f_ops && filp->f_ops->f_write )
	{
		i = filp->f_ops->f_write( filp, __buf, len );
	}else{
		printk("ramfs_write null point\n",fd);
	}
	
	return (i);
}

int ramfs_close( int fd )
{
	struct file *filp;
	int i = 0;
		
	filp = current->file[fd];	
	
	if ( filp && filp->f_ops && filp->f_ops->f_close )
	{
		i = filp->f_ops->f_close( filp );
	}
	
	return (i);
}

int ramfs_init(void)
{
	printk("ramfs_init() ...\n");
	ramfile_init();
	ramfs_create_dir("/",DIR);
	
	vfs_filesystem_register(&ramfs);
	vfs_filesystem_mount("/",&ramfs );
	printk("ramfs_init() done\n");
	
	return 0;
}