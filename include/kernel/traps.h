#ifndef __TRAPS_H__
#define __TRAPS_H__

#define MAX_IDT_NR   256

extern void kernel_panic ( int cpl, unsigned long esp );
extern void kernel_die(const char*fmt,...);

extern void trap_init( void );

#endif
