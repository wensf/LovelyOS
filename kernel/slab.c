#include <page.h>
#include <types.h>
#include <slab.h>
#include <sched.h>
#include <printk.h>

struct slab_struct
{
	struct slab_struct* next;
	struct slab_struct* prev;

	uint32 addr;			// 表示slab管理的内存首地址
	uint32 size;			// 表示slab管理的内存长度
	uint32 granularity;		// 表示slab管理的内存粒度，单位为字节
	uint32 bitmap;			// 表示slab管理的内存使用情况
};

struct slab_link
{
	struct slab_struct* slab;
};


struct slab_link m_slab;

int slab_init(void)
{
	int i;
	unsigned long page;
	struct slab_struct *slab;
	
	page = get_free_page();
	if ( !page ){
		return (-1);
	}
	slab = (struct slab_struct*)(page-sizeof(struct slab_struct));
	slab->addr = page;
	slab->size = PAGE_SIZE;
	slab->granularity = 128;
	slab->bitmap = 0;
	i = (PAGE_SIZE / slab->granularity) - 1; // used by slab
	slab->bitmap = 1<<i;

	m_slab.slab = slab;

	return 0;
}

uint8 *slab_alloc( int size )
{
	struct slab_struct *slab;
	uint32 gran;

	slab = m_slab.slab;

	if ( size > slab->granularity ){
		return (0);
	}
	gran = slab->granularity;

	for ( int i = 0; i < slab->size/gran; i++ ){
		if ( !(slab->bitmap & (1<<i)) ){
			slab->bitmap |= 0x1<<i;
			return (uint8*)(slab->addr + (i*gran));
		}
	}

	return (0);
}

int slab_free(uint32 addr)
{
	struct slab_struct *slab;
	int idx;
	
	slab = m_slab.slab;
	idx = (addr-slab->addr)/slab->granularity;

	if ((addr>= slab->addr) && (addr < (slab->addr+PAGE_SIZE)))
	{
		slab->bitmap &= ~(1<<idx);
	}else{
		return (-1);
	}
	
	return (0);
}

