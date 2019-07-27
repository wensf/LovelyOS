#ifndef __PAGE_H__
#define __PAGE_H__

#include <types.h>

#define P_P   1<<0
#define P_RW  1<<1
#define P_US  1<<2


#define PAGING_MEM_SIZE   6*1024*1024
#define PAGING_MEM_ITEM   ((PAGING_MEM_SIZE)>>12)
#define MEM_USED  1


// #define PAGE_DEBUG_ENABLE					0

extern void page_init(void);
extern int page_map( uint32 pgd,uint32 pa, uint32 va, int size, int attr );
extern int page_attrs_set( uint32 pgd, uint32 va, int attrs );
extern void do_page_fault(void);

extern int mem_init( unsigned long mem_start, unsigned long mem_end );
extern unsigned long get_free_page(void);
extern unsigned long get_free_pages( int page_nr );
extern void free_page( unsigned long addr );

#define MAP_NR(addr) (((addr)-low_memory_start)>>12)


extern unsigned char mem_map[];
extern unsigned long low_memory_start, low_memory_end;


#endif
