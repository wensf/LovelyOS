
#include "sched.h"
#include "buffer.h"

/**
 * Temp memory alloc as array
 */
struct task_struct buff[TASK_NR*2];
unsigned long kernel_stack[TASK_NR*2][64];
unsigned long user_stack[TASK_NR*2][64];

