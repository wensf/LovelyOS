#ifndef __FAT16_H__
#define __FAT16_H__

struct fat16_header						/** 定义FAT16头部 */
{
	unsigned char jmp_boot[3];          /** 3字节跳转指令 */
	unsigned char OEMNAME[8];           /** OEM名称 */
	unsigned short BytePerSector;       /** 每扇区字节数 */
	unsigned char SectorPerCluster;     /** 每簇扇区数 */
	unsigned short reserveSector;       /** 保留扇区数:扇区开始(包括分区引导扇区)至FAT前的扇区数量 */
	unsigned char NumOfFAT;             /** FAT数量 */
	unsigned short NumOfRoot;           /** 根目录项数 */
	unsigned short TotalSector16;	    /** 总扇区数 */
	unsigned char media_type;           /** 介质类型 */
	unsigned short FATSize16;           /** 一个FAT占用的扇区数 */
	unsigned short SectorPerTrk;        /** 每磁道扇区数量（硬盘) */
	unsigned short heads;               /** 磁头数量 */
	unsigned int hiddSector;            /** 隐藏扇区数:本分区前的扇区数 */
	unsigned long TotalSector32;        /** 总扇区数量 */
	unsigned char DrvNum;               /** 驱动号：硬盘0x80，软盘0x00 */
	unsigned char reserve_nt;           /** 保留字节 */
}__attribute__((packed));

struct fat16_root_item					/** 定义根目录项 */
{
	unsigned char fileName[8];          /** 文件名称 8 bytes */
	unsigned char fileExt[3];           /** 扩展名 3 bytes */
	unsigned char fileAttr;             /** 文件属性 */
	unsigned char reserve;              /** 保留字节 */
	unsigned char createTime_ms;        /** 文件创建时的时间秒后面的时间, 10ms为单位 */
	unsigned short createTime;          /** 文件创建时时分秒 */
	unsigned short createDate;          /** 文件创建日期，为年<<11+分<<5+秒+2 */
	unsigned short accessDate;          /** 文件访问日期，格式与创建日期相同 */
	unsigned short h16;                 /** 起始簇的高16位 */
	unsigned short modifiedTime;        /** 文件修改时间 */
	unsigned short modifiedDate;        /** 文件修改日期 */
	unsigned short l16;                 /** 起始簇的低16位 */
	unsigned int length;                /** 文件长度 */
}__attribute__((packed));

struct fat_item
{
	unsigned short entry;
};


extern struct fat_item fat_entry[64*512/2];

extern int driver_load_sector( int lba, int n, unsigned char *buf );

extern int show_root_item( struct fat16_root_item *p );
extern int fat16_get_short_name( struct fat16_root_item *p, char *file_name);
extern int fat16_read_fat1( struct fat16_header *fat, unsigned char *buf );
extern int fat16_read_file_data( struct fat16_header *fat, struct fat16_root_item *p, unsigned char *buf );
extern int fat16_get_file_length ( struct fat16_root_item *p );
extern int fat16_get_file_first_cluster( struct fat16_root_item *p );

#endif


