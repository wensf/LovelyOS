#ifndef __PRINTK_H__
#define __PRINTK_H__

#define COLOR_BLACK               	0X000000
#define COLOR_WHITE               	0XFFFFFF
#define COLOR_GREEN               	0X00FF00
#define COLOR_BLUE                	0X00FF00

#define CCOF		           		0x505050
#define CCOB        		   		0x101010


extern int printk(const char *fmt,...);

#endif
