/**
 * File os/include/mm/memory.h
 * 2016.09.08
 */
#ifndef __MEMORY_H__
#define __MEMORY_H__

extern int mem_init( unsigned long mem_start, unsigned long mem_end );
extern unsigned long get_free_page(void);
extern unsigned long get_free_pages( int page_nr );
extern void free_page( unsigned long addr );

#endif
