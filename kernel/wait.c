#include <syscall.h>
#include <wait.h>
#include <sched.h>
#include <system.h>
#include <syscall.h>
#include <printk.h>


int do_wait(int *exit_code)
{
	printk("\n\n\n\n\n\ndo_wait()...............\n");

	current->state = TASK_WAIT;
	while ( !(current->signal & 0x1)){
		schedule();
	}

	for ( int i = 0; i < SIZEOF_NR(task); i++ ){
		if ( task[i] && task[i] != current &&
			task[i]->state == TASK_STOP ){
			*exit_code = task[i]->exit_code; 
			printk("wait() exit...............\n");
			return i;
		}
	}
	
	return (-1);	// Error
}

int syscall_wait(int *exit_code)
{
	return do_wait(exit_code);
}

