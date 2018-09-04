#ifndef __TTY_H__
#define __TTY_H__

#include <fs/vfs.h>

#define NR_TTY					 			  	 4
#define TTY_NAME_LEN            				16

extern int tty_init(void);

#endif
