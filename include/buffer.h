#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "sched.h"

extern struct task_struct buff[TASK_NR];
extern unsigned long kernel_stack[TASK_NR][KERNEL_STACK_SIZE];
extern unsigned long user_stack[TASK_NR][USER_STACK_SIZE];

#endif
