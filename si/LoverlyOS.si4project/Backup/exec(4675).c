/**
 * os/fs/exec.c
 * Author : Wen Shifang
 * 2019.07.02
 */

#include <types.h>
#include <sched.h>
#include <printk.h>

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

extern void new_task_entry(void);


int syscall_execve( uint32 *eip, const uint8 *file_name)
{
	// 进程在fork()时就已经具备独立的内核栈与用户栈.
	// 调用do_execve()时将两个堆栈指针恢复到栈顶.
	// 在调用execve时，压栈的地址为返回地址即EIP，当进入_syscall_execve后，EIP在
	// 内核栈的偏移位置为56字节.因此在_syscall_execve中,通过指令lea EIP(%esp),%eax 也即lea 56(%esp),%eax
	// 将返回地址存放在%eax寄存器中,然后将其压入syscall_execve()的栈帧内，得到的参数便是eip
	// 通知指针eip可以直接修改内核栈上对应EIP位置上的值.
	// 我们正是通过修改内核栈保存的EIP值达到切换系统调用返回后，直接执行新进程指令的目的.
	// 使用新程序的入口entry填入eip

	printk("FilePath=%s\n",file_name);
	printk("EIP = %08x\n", (uint32)*eip);

	eip[0] = new_task_entry;

	return (0);
}



