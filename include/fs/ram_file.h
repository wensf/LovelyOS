#ifndef __RAM_FILE__
#define __RAM_FILE__

#include <types.h>
#include <fs/vfs.h>

#define MAX_NAME    		16
#define MAX_RAM_FILE  		64

#define F_TYPE_FILE         0
#define F_TYPE_DIR          1

#define RAM_FILE_UNLOCK     0
#define RAM_FILE_LOCK       1

typedef struct _ram_file
{
	uint32 f_type;
	int32  f_id;
	int32  f_father;
	uint32 f_ctime;
	uint32 f_atime;
	uint32 f_mtime;
	uint32 f_base;
	int32  f_size;
	int32  f_pos[2];
	int32  f_count;
	int32  f_lock;
	struct file_operations *f_ops;
	int8   f_name[MAX_NAME];
}ram_file;

extern uint32 last_fid;
extern ram_file ram_files[MAX_RAM_FILE];

extern int ramfile_init(void);
extern int ramfile_create_dir ( int32 father_fid, uint32 ctime, const int8 *name );
extern int ramfile_create_file ( int32 father_fid, uint32 ctime, const int8 *name, struct file_operations *f_ops, int attrs);
extern int32 ramfile_fid_find ( const int8 *path_name );
extern int32 ramfile_fid_get (void);
extern struct file_operations *ram_file_get_fops( int32 f_id );

#endif
