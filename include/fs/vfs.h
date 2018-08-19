#ifndef __VFS_H__
#define __VFS_H__

#define FILE_SYSTEM_NR              8   // support 8 file system mount in max.
#define FSAL_MAX_OPENED_FILE        8   // support 8 files opened.

#define O_RD                        1
#define O_WR                        2
#define O_RDWR                      O_RD|O_WR
#define O_CREAT                     4


struct struct_filesystem
{
	int fs_type;
};

struct inode
{
	int identifier;
	int i_mode;
	void *private_data;
};

struct file;

struct file_operation
{
	int (*f_open)( struct inode *inode, struct file *filp );
	int (*f_read)( struct file *filp, char *__buf, int len );
	int (*f_write)( struct file *filp, const char *__buf, int len );
	int (*f_close)( struct file *filp );
};

struct file
{
	struct file_operation *f_ops;
	int w_o;
	int r_o;
};

extern int vfs_init(void);

extern struct inode* filp_open( const char *__file_name, int mode, int flags );
extern int filp_read( struct file *filp, char * __buf, int len );
extern int filp_write( struct file *filp, const char * __buf, int len );
extern int filp_close( struct file *filp );

#endif
