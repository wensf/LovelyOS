#include <syscall.h>
#include <printk.h>
#include <exit.h>
#include <sched.h>


int do_exit(int *exit_code)
{
	struct task_struct *p;


	printk("task[%d] do_exit....\n", current->pid);

	p = current->parent;
	
	current->state = TASK_STOP;
	printk("send signal to parent[%d]\n", p->pid);
	if (p)
	{
		p = task[0];
	}
	p->signal |= (0x1<<0);	// signal to parent task.

	

if_return:
		schedule();
	goto if_return;
	
	return (0);
}

int syscall_exit ( int *exit_code )
{
	return do_exit(exit_code);		
}



