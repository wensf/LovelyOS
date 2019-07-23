#include <syscall.h>
#include <types.h>
#include <wait.h>
#include <sched.h>
#include <system.h>
#include <syscall.h>
#include <printk.h>
#include <page.h>


int do_wait(int *exit_code)
{
//	struct vmb *m;
	int i;
	
	current->state = TASK_WAIT;
	while ( !(current->signal & 0x1)){
		schedule();
	}

	for ( i = 0; i < SIZEOF_NR(task); i++ ){	// 寻找任意一个退出的子(孙)任务.
		if ( task[i] && task[i] != current &&
			task[i]->state == TASK_STOP ){
			*exit_code = task[i]->exit_code; 
			break;
		}
	}

	if ( i < SIZEOF_NR(task) ){
//		m = task[i]->mm.code_start;		// 释放任务的代码段，数据段，用户栈，内核栈.
//		while ( m != NULL ){
//			free_page(mm->code_start);
//			m = m->next;
//		}
		free_page(task[i]->pgd);		// 页目录
		free_page((uint32)task[i]);				// 内核栈
		task[i] = 0;

		current->signal &= ~(1<<0);

		printk("task[%d] exit do_wait()\n", current->pid);

		return (i);		// PID
	}

	printk("task[%d] exit do_wait()\n", current->pid);
	
	return (-1);	// Error
}

int syscall_wait(int *exit_code)
{
	return do_wait(exit_code);
}

