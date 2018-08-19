#ifndef __CONSOLE_H__
#define __CONSOLE_H__

extern int x, y;

extern void console_init(void);

extern void putc( int x, int y, char c);
extern int get_position( int *x, int *y );

#endif
