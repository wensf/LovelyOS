#ifndef __RAMFS_H__
#define __RAMFS_H__

#include <fs/vfs.h>

extern int ramfs_init(void);
extern int ramfs_create_file( const char *path, struct file_operations *f_ops, int flags);
extern int ramfs_open( const char *path, int mode, int flags );
extern int ramfs_read( int fd, char *__buf, int len );
extern int ramfs_write( int fd, const char *__buf, int len );
extern int ramfs_seek( int fd, int origin, int offset );
extern int ramfs_close( int fd );


#endif
