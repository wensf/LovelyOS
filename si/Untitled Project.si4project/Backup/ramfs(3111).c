#include <fs/ramfs.h>
#include <fs/vfs.h>
#include <fs/ram_file.h>
#include <traps.h>
#include <types.h>
#include <sched.h>
#include <printk.h>
#include <system.h>

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

	if ( file_last >= SIZEOF_NR(file_table)){
		kernel_die("file_alloc() oom!!!");
	}

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

	dstr[i] = '\0';

	return (i);
}

int ramfs_open( const char *path, int mode, int flags )
{
	struct file *filp;
	ram_file *ram_filp;
	int32 f_id;

	printk("in ramfs_open addr=0x%08x,path=%s, mode=%d, flags=%d, sp=0x%08x\n",(uint32)path,(uint32)path,mode,flags,&f_id);

	f_id = ramfile_fid_find(path);
	if ( (f_id < 0) && !(mode & O_CREAT) )
	{	
		kernel_die("task[%d] not found at 0x%08x -> %s, mode=%d, flags=%d, sp=0x%08x\n", current->pid,(uint32)path,(uint32)path, mode, flags, &f_id);
		return (-1);
	}

	if ( f_id > (MAX_RAM_FILE-1) ){
		kernel_die("task[%d] ramfs_open() error f_id=%d\n",current->pid, f_id);
	}
	
	#if 0
	if ( mode|O_CREAT )
	{
		/* open with O_CREATE
		 * Do nothing now */
		return (-1);
	}
	#endif
		
	ram_filp = &ram_files[f_id];
	
	filp = file_alloc();
	if ( !filp )
	{
		kernel_die("task[%d] ram_files at 0x%08x ram_filp=%08x, f_open=%08x\n",current->pid,(uint32)&ram_files,ram_filp->f_ops,ram_filp->f_ops->f_open);
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

	printk("ramfs_open %s,ram_files at 0x%08x ram_filp=%08x, f_open=%08x\n",(uint32)path,(uint32)&ram_files,ram_filp->f_ops,ram_filp->f_ops->f_open);	
	
	current->file[last_unused] = filp;
	current->file_counter[last_unused]++;
	
	if ( filp->f_ops && filp->f_ops->f_open )
 	{	
		filp->f_ops->f_open(filp);
	}else{
		kernel_die("task[%d] no f_open to call\n", current->pid);
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
		i = filp->f_ops->f_read(filp,__buf, len);
	}
	
	return (i);
}

int ramfs_lseek( int fd, int offset, int whence )
{
	struct file *filp;
	int i = -1;

	if ( fd < 0 ){
		kernel_die("task[%d],ramfs_lseek() fd=%d sstop=0x%08x\n", current->pid,fd,&fd);
	}

	filp = current->file[fd];

	#if 0
	printk("task[%d] ramfs_lseek() fd=%d,offset=%d, whence=%d    ", current->pid, fd, offset, whence);
	printk("filp=%08x, filp->f_ops=%08x, f_lseek=%08x  sstop=0x%08x ",filp, filp->f_ops, filp->f_ops->f_lseek,&fd);
	#endif
	if ( !filp ){
		kernel_die("task[%d],ramfs_lseek() filp=0x%08x sstop=0x%08x\n", current->pid,filp,&fd);
	}
	
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
		kernel_die("task[%d], ramfs_write [%d] %s:%d\n",current->pid, fd, __FILE__,__LINE__);
	}
	
	filp = current->file[fd];

	// printk("task[%d], ramfs_write [%d] %s:%d\n",current->pid, fd, __FILE__,__LINE__);

	if ( filp && filp->f_ops && filp->f_ops->f_write )
	{
		i = filp->f_ops->f_write( filp, __buf, len );
	}else{
		kernel_die("task[%d], ramfs_write [%d] null point\n",current->pid,fd);
	}
	
	return (i);
}

unsigned char *ramfs_mmap(int fd, int size, int flags)
{
	struct file *filp;

	// kernel_die("task[%d], just stop on here fd=%d, size=%d, flags=%d\n",current->pid, fd, size, flags);

	if ( (fd < 0) || (fd > MAX_RAM_FILE-1) )
	{
		kernel_die("task[%d], ramfs_mmap [%d] %s:%d error: fd=%d\n",current->pid, fd, __FILE__,__LINE__);
	}

	filp = current->file[fd];

	if ( !filp ){
		kernel_die("task[%d], ramfs_mmap() filp=0x%08x sstop=0x%08x\n", current->pid,filp,&fd);
	}	
			
	if ( filp && filp->f_ops && filp->f_ops->f_mmap )
	{
		return filp->f_ops->f_mmap( filp, size, flags );
	}else{
		kernel_die("task[%d], ramfs_mmap [%d] null point\n",current->pid,fd);
	}
	return (0);
}

int ramfs_ioctl( int fd, int cmd, int arg)
{
	struct file *filp;

	// kernel_die("task[%d], just stop on here fd=%d, cmd=%d, arg=%d\n",current->pid, fd, cmd, arg);

	if ( (fd < 0) || (fd > MAX_RAM_FILE-1) )
	{
		kernel_die("task[%d], ramfs_ioctl [%d] %s:%d error: fd=%d\n",current->pid, fd, __FILE__,__LINE__);
	}

	filp = current->file[fd];

	if ( !filp ){
		kernel_die("task[%d], ramfs_ioctl() filp=0x%08x sstop=0x%08x\n", current->pid,filp,&fd);
	}	
			
	if ( filp && filp->f_ops && filp->f_ops->f_ioctl )
	{
		return filp->f_ops->f_ioctl( filp, cmd, arg );
	}else{
		kernel_die("task[%d], ramfs_ioctl [%d] null point\n",current->pid,fd);
	}	

	return (-1);
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
	current->file[fd] = 0;
	
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
