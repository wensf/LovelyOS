#ifndef __WAIT_QUEUE_H__
#define __WAIT_QUEUE_H__

struct wait_queue_node
{
	struct wait_queue_node *next;
	struct task_struct *task;
};
struct wait_queue
{
	struct wait_queue_node *head;
	struct wait_queue_node *tail;
};

extern int sleep_on( struct wait_queue *queue );
extern int wake_up (struct wait_queue *queue);

#endif

