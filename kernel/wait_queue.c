#include <wait_queue.h>
#include <traps.h>
#include <sched.h>
#include <printk.h>
#include <slab.h>


/**
 * int sleep_on( struct wait_queue *queue );
 * make current to sleep on the queue.
 */

int sleep_on( struct wait_queue *queue )
{
	struct wait_queue_node *node;

	node = (struct wait_queue_node *)slab_alloc(sizeof(struct wait_queue_node));
	if ( !node ){
		kernel_die("sleep_on() oom!!!\n");
	}
	node->next = queue->head->next;
	node->task = current;

	queue->head->next = node;

	current->state = TASK_WAIT;
	schedule();

	return (0);
}

/**
 * int wake_up (struct wait_queue *queue);
 * wake up the task on the head of queue.
 */

int wake_up (struct wait_queue *queue)
{
	struct wait_queue_node *node, *q;
	struct task_struct *p;
	
	if ( queue->head->next )
	{
		node = queue->head->next;
		p = node->task;
		if ( !p ){
			kernel_die("wake_up fatal error\n");
		}
		p->state = TASK_RUNNING;

		q = node->next;
		slab_free((uint32)node);
		queue->head->next = q;
	}
	
	return (0);
}



