#include <syscall.h>
#include <printk.h>
#include <exit.h>
#include <sched.h>


int do_exit(int exit_code)
{
	struct task_struct *p;
	p = current->parent;
	
	current->state = TASK_STOP;
	current->exit_code = exit_code;
	if (!p){
		p = task[0];
	}
	printk("send signal to parent [%d]\n", p->pid);
	p->signal |= (0x1<<0);	// signal to parent task.
	if ( p->state == TASK_WAIT ){
		p->state = TASK_RUNNING;
	}
if_return:
		schedule();
	goto if_return;
	
	return (0);
}

int syscall_exit ( int exit_code )
{
	return do_exit(exit_code);		
}



