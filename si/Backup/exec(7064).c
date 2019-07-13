/**
 * os/fs/exec.c
 * Author : Wen Shifang
 * 2019.07.02
 */

#include <types.h>
#include <sched.h>

extern void kernel_stack_dump(struct regs *reg);

/**
 *
 * syscall_execve() to execute a new program.
 */

#if 0
		EIP = 0x20	/* EIP save at kernel stack in offset 0x20 */
	.align 4
	_syscall_execve:
		pushl %ebx	/* EBX point to the param file_name */
		lea EIP(%esp),%eax
		pushl %eax	/* EIP push as param for call syscall_execve */
		call syscall_execve
		addl $8,%esp
		ret

#endif


int syscall_execve( uint32 *eip, const uint8 *file_name)
{
	// 进程在fork()时就已经具备独立的内核栈与用户栈.
	// 调用do_execve()时将两个堆栈指针恢复到栈顶
	// 
	// 使用新程序的入口entry填入eip

	struct regs *pregs;

	pregs = (struct regs *)eip;

	printk("FilePath=%s\n",file_name);
	printk("EIP = %08x\n", (int32)eip);
	kernel_stack_dump(pregs);

	return (0);
}

