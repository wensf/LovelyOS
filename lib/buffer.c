
#include "sched.h"
#include "buffer.h"

/**
 * Temp memory alloc as array
 */
struct task_struct buff[TASK_NR];
unsigned long kernel_stack[TASK_NR][KERNEL_STACK_SIZE];
unsigned long user_stack[TASK_NR][USER_STACK_SIZE];

