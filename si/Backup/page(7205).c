#include <types.h>
#include <mm/page.h>
#include <init.h>
#include <memory.h>

#define P_P   1<<0
#define P_RW  1<<1
#define P_US  1<<2

extern struct kernel_param *kparam;

void page_init(void)
{
	/**
	 * mapping the Vedio RAM from it's phyaddr to vaddress
	 */
	
	page_map( kparam->vaddr, kparam->vaddr, 0x800000, P_P|P_RW|P_US);
}

int page_map( uint32 pa, uint32 va, int size, int attr )
{
	if ( size < 4096 ) size = 4096;
	
	for ( int i = 0; i < size/4096; i++ )
	{
		page_map_a( pa+i*4096, va+i*4096, attr );
	}	
	
	return 0;
}

int page_map_a ( uint32 pa, uint32 va, int attr )
{
	uint32 *pdir = (uint32 *)0x100000;
	uint32 *ptable;

	uint32 pdir_entry = va >> 22;
	uint32 ppte_entry = (va >> 12 ) & 0x3FF;

	if ( *(pdir + pdir_entry ) == 0 )
	{
		ptable = (uint32 *) get_free_page();
		*(pdir + pdir_entry )  = ((uint32)ptable & 0xFFFFF000) + attr;
	}else{
		ptable = (uint32*)(*(pdir + pdir_entry ) & 0xFFFFF000);
	}

	*(ptable+ppte_entry) = (pa & 0xFFFFF000) + attr;

	return (0);
}


