#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "sched.h"

extern struct task_struct buff[];
extern unsigned long kernel_stack[TASK_NR*2][64];
extern unsigned long user_stack[TASK_NR*2][64];

#endif
