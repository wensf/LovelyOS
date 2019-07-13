/**
 * File : os/kernel/memory.c
 * 2016.09.05
 * last modify : 2018.9.2
 */
#include <printf.h>
#include <memory.h>
#include <printk.h>

#define MEM_LOW           (2*1024*1024)
#define PAGING_MEM_SIZE   8*1024*1024
#define PAGING_MEM_ITEM   ((PAGING_MEM_SIZE)>>12)
#define MEM_USED  1

static unsigned char mem_map[PAGING_MEM_ITEM];

int mem_init( unsigned long mem_start, unsigned long mem_end )
{
    int i;
    /**
     * make all main memory eara to used firstly.
     */
    for ( i = 0; i < PAGING_MEM_ITEM; i++)
    {
        mem_map[i] = MEM_USED;
    }

    /**
     * make available memory eara to 0.
     */
    for ( i = (mem_start-MEM_LOW)>>12; i < (mem_end-MEM_LOW)>>12; i++)
    {
        mem_map[i] = 0;
    }

    return 0;
}

/**
 * To get a free page from physical memory, and mark it
 * used. If no free pages left, return 0.
 */
unsigned long get_free_page(void)
{
    int i;

    for ( i = 0; i < PAGING_MEM_ITEM; i++)
    {
        if ( !mem_map[i] )
        {
            mem_map[i]++;
            return MEM_LOW + (i* 4096);
        }
    }

    return 0;
}

unsigned long get_free_pages( int page_nr )
{
	int flg = 0, cnt = 0, pidx = 0;

	for ( int i = 0; i < PAGING_MEM_ITEM && !flg; i++ )
	{
		if ( mem_map[i] ){ continue; }

		cnt = 1;
		for ( int j = i+1, k = 1; (k < page_nr) && (j < PAGING_MEM_ITEM); j++,k++ )
		{
			if ( !mem_map[j] )
			{
				cnt++;
			}else{
                i = j;
				break;
			}

			if ( cnt == page_nr )
			{
				pidx = i; flg = 1;
				break;
			}
		}
	}

	if ( flg == 1 )
	{
		for ( int i = pidx; i < pidx+page_nr; i++ )
		{
			mem_map[i]++;
		}

//		printk("alloc page start %08x, page_nr=%d\n", MEM_LOW + (pidx* 4096), page_nr);
		return MEM_LOW + (pidx* 4096);
	}else{
		return (0);
	}
}

/**
 * To free a page, make it unused.
 * @addr the page base address.
 * @return none
 */

void free_page( unsigned long addr )
{
    addr -= MEM_LOW;
    addr >>= 12;

    if ( addr < PAGING_MEM_ITEM )
    {
        if ( mem_map[addr] > 0 ){ mem_map[addr]-- ;}
    }else
    {
        for(;;) ;
    }
}

/**
 * To free pages, make it unused.
 * @addr the page base address.
 * @page_nr the number of page.
 * @return none
 */
void free_pages( unsigned long addr, int page_nr )
{
	unsigned long va;
	int i;

	i = 0; va = addr;
	while ( i < page_nr )
	{
		va -= MEM_LOW;
		va >>= 12;
		if ( mem_map[va] >0 ) mem_map[va]--;
		i++;
		va = addr + i*4096;
	}
}
