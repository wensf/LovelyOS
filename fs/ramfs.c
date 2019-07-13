#include <fs/ramfs.h>
#include <fs/vfs.h>
#include <fs/ram_file.h>
#include <traps.h>
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

unsigned char get_fs_byte(const char * addr)
{
	unsigned register char _v;

	__asm__ ("movb %%fs:%1,%0":"=r" (_v):"m" (*addr));
	return _v;
}

int get_fs_string(char *dstr,const char *sstr)
{
	char c;
	const char *p;
	int i;

	p = sstr;
	i = 0;
	do{
		c = get_fs_byte(p++);
		dstr[i++] = c;
	}while( c != '\0' );

	return (i);
}

int ramfs_open( const char *path, int mode, int flags )
{
	struct file *filp;
	int32 f_id;

//	char file_name[MAX_NAME];
//	get_fs_string(file_name,path);
	
//	printk("in ramfs_open addr=%08x,path=%s, mode=%d, flags=%d\n",(uint32)path,path,mode,flags);
	
	f_id = ramfile_fid_find((const int8*)path);
	if ( (f_id < 0) && !(mode & O_CREAT) )
	{	
		
		kernel_die("not found at %08x -> %s\n", (uint32)path,path);
		return (-1);
	}else{
		printk("fid =%d\n", f_id);
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
	printk("ram_files at %08x ram_filp=%08x, f_open=%08x\n",(uint32)&ram_files,ram_filp->f_ops,ram_filp->f_ops->f_open);
	
	filp = file_alloc();
	if ( !filp )
	{
		kernel_die("file_alloc failed\n");
		return (-1);
	}
	
	filp->f_ops = ram_filp->f_ops;
	filp->r_o = ram_filp->f_pos[0];
	filp->w_o = ram_filp->f_pos[1];
	
	int last_unused = get_last_unused();
	if ( last_unused == -1 )
	{
		file_free(filp);
		kernel_die("last_unused failed\n");
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

	if ( fd > MAX_RAM_FILE-1 ){
		kernel_die("task[%d], ramfs_write %[%d] %s:%d error: fd=%d\n",current->pid, fd, __FILE__,__LINE__);
	}
	
	filp = current->file[fd];
	
	if ( filp && filp->f_ops && filp->f_ops->f_write )
	{
		i = filp->f_ops->f_write( filp, __buf, len );
	}else{
		kernel_die("task[%d], ramfs_write [%d] null point\n",current->pid,fd);
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

	file_free(filp);
	
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