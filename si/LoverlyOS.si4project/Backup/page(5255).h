#ifndef __PAGE_H__
#define __PAGE_H__

#include <types.h>

extern void page_init(void);
extern int page_map( uint32 pa, uint32 va, int size, int attr );
extern int page_map_a ( uint32 pa, uint32 va, int attr );

#endif
