#ifndef __PAGE_H__
#define __PAGE_H__

#include <types.h>

#define P_P   1<<0
#define P_RW  1<<1
#define P_US  1<<2

extern void page_init(void);
extern int page_map( uint32 pgd,uint32 pa, uint32 va, int size, int attr );
extern int page_attrs_set( uint32 pgd, uint32 va, int attrs );
extern void do_page_fault(void);

#endif
