/**
 * File fat16.c
 * implement the FAT16 filesytem basic api
 * Author : Wen Shifang
 * 772148609@qq.com
 * Create 2018.07.17
 * Last Modify : 2018.07.17
 */

#include <fat16.h>
#include <libc.h>
#include <printf.h>

__asm__(".code16gcc");

struct fat_item fat_entry[64*512/2];

struct lba_struct { 
	unsigned char psize;
	unsigned char reserved;
	unsigned short bcount;
	unsigned short offset;	
	unsigned short segment;
	unsigned long long blockNum;
} __attribute__((packed));

/**
 * int Driver_Load_Sector( int sector_nr, int n, unsigned char buf );
 * 用于从磁盘中读取扇区数据
 * @ sector_nr 逻辑扇区号
 * @ n 与读取的扇区个数
 * @ buf 用于存储数据的缓冲区
 */
  
struct lba_struct lba_info;  
  
int driver_load_sector( int lba, int n, unsigned char *buf )
{
	int _ret;
		
	lba_info.psize    = 16;
	lba_info.bcount   = n;
	lba_info.segment  = 0x9000;
	lba_info.offset   = ((unsigned long)buf) & 0xFFFF;
	lba_info.blockNum = lba;
	
	
	printf("lba size = %d bytes, addr = %08x\n", sizeof(lba_info),(unsigned int)&lba_info);
	
	printf("size %d, count %d, seg %04x, offset %04x, block %lld\n",
			lba_info.psize,
			lba_info.bcount,
			lba_info.segment,
			lba_info.offset,
			lba_info.blockNum );
	
	unsigned int p = (unsigned int )&lba_info;
	
	__asm__ __volatile__(
	 	"push    %%bx\n\t"
		"push    %%di\n\t"
		"push    %%si\n\t"
		"push    %%bp\n\t"
		"mov     %%sp, %%bp\n\t"
		"mov     %1, %%eax\n\t"
		"mov     %%ax, %%si\n\t"		
		"mov     $0x80, %%dl\n\t"
		"mov     $0x42, %%ah\n\t"
		"int     $0x13\n\t"
		"jc      error\n\t"
		"jnc     done\n\t"
		"error:\n\t"
		"movl    $0xFFFFFFFF,%0\n\t"
		"done:\n\t"
		"mov     %%bp, %%sp\n\t"
		"pop     %%bp\n\t"
		"pop     %%si\n\t"
		"pop     %%di\n\t"
		"pop     %%bx\n\t"
		:"=a"(_ret)
		:"a"(p)
	);

	return _ret;
}

int show_root_item( struct fat16_root_item *p )
{
	char file_name[9],fileName[9];
	char file_ext[4];

	if ( !p )
	{
		return (-1);
	}

	memset(file_name,0,sizeof(file_name));
	memset(fileName,0,sizeof(fileName));
	memset(file_ext,0,sizeof(file_ext));

	strncpy(file_name, (char*)p->fileName, 8);
	strncpy(file_ext, (char*)p->fileExt, 3);

	trim(file_name,fileName);

	/** 文件创建时间 */
	char ctime[64],mtime[64],atime[64];
	int year  = (p->createDate>>9) + 1980;
	int month = (p->createDate>>5) & 0x0F;
	int day   = (p->createDate) & 0x1F;

	int msec   = p->createTime_ms * 10;
	int hour   = p->createTime>>11;
	int minute = (p->createTime>>5) & 0x3F;
	int second = (p->createTime & 0x1F) * 2 + msec/1000;

	sprintf(ctime, "%d年%d月%d日 %d时%d分%d秒",year,month,day, hour,minute,second);

	/** 文件修改时间 */
	year  = (p->modifiedDate>>9) + 1980;
	month = (p->modifiedDate>>5) & 0x0F;
	day   = (p->modifiedDate) & 0x1F;

	hour   = p->modifiedTime>>11;
	minute = (p->modifiedTime>>5) & 0x3F;
	second = (p->modifiedTime & 0x1F) * 2;

	sprintf(mtime, "%d年%d月%d日 %d时%d分%d秒",year,month,day, hour,minute,second);

	/** 最后访问时间 */
	year  = (p->accessDate>>9) + 1980;
	month = (p->accessDate>>5) & 0x0F;
	day   = (p->accessDate) & 0x1F;

	sprintf(atime, "%d年%d月%d日",year,month,day);

	printf("%s.%s %d bytes %s %s %s\n", fileName,file_ext,p->length, ctime, mtime, atime);

	printf("起始簇号:%d\n", (p->h16<<16) | p->l16 );

	return 0;
}

int fat16_get_short_name( struct fat16_root_item *p, char *file_name)
{
	char name[9],fileName[9];
	char ext[4],fileExt[4];

	if ( !p )
	{
		return (-1);
	}

	memset(name,0,sizeof(name));
	memset(fileName,0,sizeof(fileName));
	memset(ext,0,sizeof(ext));
	memset(fileExt,0,sizeof(fileExt));

	strncpy(name, (char*)p->fileName, 8);
	strncpy(ext, (char*)p->fileExt, 3);

	trim(name,fileName);
	trim(ext,fileExt);

	strcpy( file_name, fileName);

	char *r = file_name;
	while ( *r++ != '\0' );
	*r++ = '.';
	for (  int i = 0; i < 3; i++ )
	{
		*r++ = fileExt[i];
	}		

	*r = '\0';

	return 0;
}

/**
 * 读取FAT1的内容到内存中以便快速查找.
 * @fat FAT16 DBR头指针
 * @buf 用于存放 FAT1的缓冲区指针
 */
int fat16_read_fat1( struct fat16_header *fat, unsigned char *buf )
{
	int fat_table_entry_sector;
	
	fat_table_entry_sector = fat->hiddSector + fat->reserveSector;

	for ( int i = 0; i < fat->FATSize16; i++ )
	{
		driver_load_sector( fat_table_entry_sector+i, 1, buf + i*512 );
	}

	return 0;
}

/**
 * 根据文件参数从磁盘读取文件内容
 * @p 目录项指针
 * @buf 用于存放文件数据的缓冲区.
 */
int fat16_read_file_data( struct fat16_header *fat, struct fat16_root_item *p, unsigned char *buf )
{
	int fat_table_entry_sector;
	int root_directory_sector;
	int data_start_sector;

	fat_table_entry_sector = fat->hiddSector + fat->reserveSector;
	root_directory_sector = fat_table_entry_sector + fat->FATSize16 * fat->NumOfFAT;
	data_start_sector = root_directory_sector + (fat->NumOfRoot * 32)/512;

	int sector_nr;
	int cluster_head;

	cluster_head = fat16_get_file_first_cluster(p);

	int i = 0;

	while ( (fat_entry[cluster_head].entry != 0xFFFF)
		&& (fat_entry[cluster_head].entry != 0x0) )
	{
		// 计算簇号 cluster_head 对应的扇区号
		sector_nr = data_start_sector + (cluster_head-2) * fat->SectorPerCluster;

		// 从磁盘读取sector_nr号扇区的数据
		driver_load_sector( sector_nr, 1, buf + i*512 );

		printf("读取底%d号扇区\n", sector_nr);

		// 继续取下一簇号
		cluster_head = fat_entry[cluster_head].entry; i++;
	}

	printf("共读取了%d个簇, %d个字节\n", i, i*512);

	return 0;
}

/**
 * 获取文件信息中的文件长度
 * @p 目录项指针
 * @return 文件内容长度
 */
int fat16_get_file_length ( struct fat16_root_item *p )
{
	return p->length;
}

int fat16_get_file_first_cluster( struct fat16_root_item *p )
{
	return (p->h16<<16) | (p->l16);
}
