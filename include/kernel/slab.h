#ifndef __SLAB_H__
#define __SLAB_H__

#include <types.h>

extern int slab_init(void);
extern uint8 *slab_alloc( int size );
extern int slab_free(uint32 addr);

#endif
