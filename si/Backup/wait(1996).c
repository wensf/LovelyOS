#include <syscall.h>
#include <wait.h>
#include <sched.h>
#include <system.h>
#include <syscall.h>
#include <printk.h>


int do_wait(int *exit_code)
{
	printk("\n\n\n\n\n\ndo_wait()...............\n");

	while ( !(current->signal & 0x1)){
		schedule();
	}

	for ( int i = 0; i < SIZEOF_NR(task); i++ ){
		if ( task[i] && task[i] != current &&
			task[i]->state == TASK_STOP ){
			*exit_code = task[i]->exit_code; 
			return i;
		}
	}
	
	printk("wait() exit...............\n");

	return (0);
}

int syscall_wait(int *exit_code)
{
	return do_wait(exit_code);
}

