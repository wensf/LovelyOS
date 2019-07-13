
#ifndef __SCHED_H__
#define __SCHED_H__

#include <fs/vfs.h>

#define SELECTOR_USER_CODE   (0x18 +3)
#define SELECTOR_USER_DATA   (0x20 +3)
#define SELECTOR_KERNEL_CODE 0x08
#define SELECTOR_KERNEL_DATA 0x10
#define TSS_SEGMENT          0x28

#define TASK_RUNNING        1
#define TASK_INTERRUPTABLE  2
#define TASK_SLEEP          3

#define PAGE_SIZE           4096
#define KERNEL_STACK_SIZE   PAGE_SIZE*2
#define USER_STACK_SIZE     PAGE_SIZE*4

#define HZ 100
#define TIME_INTERVAL (1193180/HZ)

#define KERNEL_STACK_PAGES 	(KERNEL_STACK_SIZE/PAGE_SIZE)
#define USER_STACK_PAGES    (USER_STACK_SIZE/PAGE_SIZE)

struct tss_struct
{
	unsigned long back_link;
	unsigned long esp0;
	unsigned short ss0,ss0_h;
	unsigned long esp1;
	unsigned short ss1,ss1_h;
	unsigned long esp2;
	unsigned short ss2,ss2_h;
	unsigned long cr3;
	unsigned long eip;
	unsigned long eflags;
	unsigned long eax;
	unsigned long ecx;
	unsigned long edx;
	unsigned long ebx;
	unsigned long esp;
	unsigned long ebp;
	unsigned long esi;
	unsigned long edi;
	unsigned short es,es_h;
	unsigned short cs,cs_h;
	unsigned short ss,ss_h;
	unsigned short ds,ds_h;
	unsigned short fs,fs_h;
	unsigned short gs,gs_h;
	unsigned long ldt;
	unsigned long trace_bitmap;
}__attribute__((packed));

struct thread_struct
{
	unsigned short ss0, ss0_h;
	unsigned long esp0;
	unsigned short ss, ss_h;
	unsigned long esp;
	unsigned long eip;
	unsigned long trace_bitmap;
};

struct task_struct
{
	int pid;
	int state;
	int delay;
	int u_time;
	int k_time;
	struct thread_struct thread;
	struct file *file[FSAL_MAX_OPENED_FILE];
	int file_counter[FSAL_MAX_OPENED_FILE];
	unsigned int pgd;
	unsigned long stack_button;
	int last_fd;
	struct task_struct *parent;
	struct task_struct *prev;
	struct task_struct *next;
};

struct regs
{
	unsigned long eax;
	unsigned long ebx;
	unsigned long ecx;
	unsigned long edx;
	unsigned long esi;
	unsigned long edi;
	unsigned long ebp;
	unsigned long fs;
	unsigned long gs;
	unsigned long ds;
	unsigned long es;
	unsigned long error; /* as a error */
	unsigned long eip;
	unsigned long cs;
	unsigned long eflags;
	unsigned long esp;
	unsigned long ss;
};


#define TASK_NR      				6

extern struct task_struct *current;

extern struct task_struct* task[];
extern void sched_init(void);

#define SCHED_DEBUG_ENABLE        	0

#endif


