/**
 * File : os/kernel/memory.c
 * 2016.09.05
 * last modify : 2018.9.2
 */
#include <printf.h>
#include <memory.h>

#define MEM_LOW           (2*1024*1024)
#define PAGING_MEM_SIZE   30*1024*1024
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
