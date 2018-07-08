
#ifndef __SCHED_H__
#define __SCHED_H__

#define SELECTOR_USER_CODE   (0x18 +3)
#define SELECTOR_USER_DATA   (0x20 +3)
#define SELECTOR_KERNEL_CODE 0x08
#define SELECTOR_KERNEL_DATA 0x10
#define TSS_SEGMENT          0x28

#define TASK_RUNNING        0
#define TASK_INTERRUPTABLE  1

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
	struct thread_struct thread;
	struct task_struct *prev;
	struct task_struct *next;
};

#define TASK_NR      8

extern struct task_struct* task[];
extern void sched_init(void);

#endif