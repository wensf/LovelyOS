#ifndef __NAME_I_H__
#define __NAME_I_H__

#include <vfs.h>

extern int open_namei( const char *__file_name, int mode, int flags, struct inode **inode );

#endif
