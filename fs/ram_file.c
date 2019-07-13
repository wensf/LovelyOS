#include <fs/ram_file.h>
#include <printk.h>
#include <libc.h>

uint32 last_fid;
ram_file ram_files[MAX_RAM_FILE];

int ramfile_init(void)
{
	memset(ram_files,0,sizeof(ram_files));
	for ( int i = 0; i < MAX_RAM_FILE; i++ )
	{
		ram_files[i].f_id = -1;
	}
	
	last_fid = 0;
	
	return 0;
}

int32 ramfile_fid_get (void);

int ramfile_create_dir ( int32 father_fid, uint32 ctime, const int8 *name )
{
	if ( father_fid >= MAX_RAM_FILE )
	{
		return (-1);
	}
	
	int fid = ramfile_fid_get();
	if ( fid == -1 )
	{
		return (-1);
	}
	ram_files[fid].f_atime = 0;
	ram_files[fid].f_base = 0;
	ram_files[fid].f_father = father_fid;
	ram_files[fid].f_id = fid;
	ram_files[fid].f_size = 0;
	ram_files[fid].f_pos[0] = 0;
	ram_files[fid].f_pos[1] = 0;
	strcpy((char*)ram_files[fid].f_name, (char*)name);
	ram_files[fid].f_type = F_TYPE_DIR;
	ram_files[fid].f_ops = 0;
	ram_files[fid].f_count = 0;
	ram_files[fid].f_lock = RAM_FILE_UNLOCK;	
	
	return fid;
}

int ramfile_create_file ( int32 father_fid, uint32 ctime, const int8 *name, struct file_operations *f_ops, int attrs)
{
	if ( father_fid > (MAX_RAM_FILE-1) )
	{
		return (-1);
	}
	
	int fid = ramfile_fid_get();
	if ( fid == -1 )
	{
		return (-1);
	}
	
	ram_files[fid].f_atime = 0;
	ram_files[fid].f_base = 0;
	ram_files[fid].f_father = father_fid;
	ram_files[fid].f_id = fid;
	ram_files[fid].f_size = 0;
	ram_files[fid].f_pos[0] = 0;
	ram_files[fid].f_pos[1] = 0;
	strcpy((char*)ram_files[fid].f_name, (char*)name);
	ram_files[fid].f_type = F_TYPE_FILE;
	ram_files[fid].f_ops = f_ops;
	ram_files[fid].f_count = 0;
	ram_files[fid].f_lock = RAM_FILE_UNLOCK;
	
	return fid;
}

int32 ramfile_fid_find ( const int8 *path_name )
{
	for ( int i = 0; i < last_fid; i++ )
	{
		if ( !strncmp( (char *)ram_files[i].f_name, (char *)path_name,MAX_NAME) )
 		{
			return ram_files[i].f_id;
		}
	}
	return (-1);
}

int32 ramfile_fid_get (void)
{
	int fid;
	
	if ( last_fid < MAX_RAM_FILE )
	{
		fid = last_fid;
		last_fid++;
	}else{
		fid = -1;
	}
	
	return fid;
}

struct file_operations *ram_file_get_fops( int32 f_id )
{
	return ram_files[f_id].f_ops;
}