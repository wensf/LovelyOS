#include <fs/sysfs.h>
#include <fs/vfs.h>
#include <fs/ramfs.h>

int cpuinfo_open( struct file *filp);
int cpuinfo_read( struct file *filp, char *__buf, int len );
static struct file_operations cpuinfo_fops = 
{
	.f_open = cpuinfo_open,
	.f_read = cpuinfo_read,
};

int cpuinfo_open( struct file *filp )
{
	return 0;
};

int cpuinfo_read( struct file *filp, char *__buf, int len )
{
	return 0;
};

int cpuinfo_init(void)
{
	return 0;
}


int meminfo_open( struct file *filp);
int meminfo_read( struct file *filp, char *__buf, int len);
static struct file_operations meminfo_fops = 
{
	.f_open = meminfo_open,
	.f_read = meminfo_read,
};

int meminfo_open(struct file *filp )
{
	return 0;
};

int meminfo_read( struct file *filp, char *__buf, int len )
{
	return 0;
};

int meminfo_init(void)
{
	return 0;
}


int sysinfo_open( struct file *filp );
int sysinfo_read( struct file *filp, char *__buf, int len );
static struct file_operations sysinfo_fops = 
{
	.f_open = meminfo_open,
	.f_read = meminfo_read,
};

int sysinfo_open( struct file *filp )
{
	return 0;
};

int sysinfo_read( struct file *filp, char *__buf, int len )
{
	return 0;
};
int sysinfo_init(void)
{
	return 0;
}

void sysfs_init(void)
{
	cpuinfo_init();
	ramfs_create_file("/cpu",&cpuinfo_fops,FILE);
	
	meminfo_init();
	ramfs_create_file("/mem",&meminfo_fops,FILE);
	
	sysinfo_init();
	ramfs_create_file("/sys",&sysinfo_fops,FILE);	
}