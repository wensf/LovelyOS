/**
 * os/fs/exec.c
 * Author : Wen Shifang
 * 2019.07.02
 */

#include <types.h>
#include <sched.h>
#include <printk.h>
#include <libc.h>
#include <mm/memory.h>
#include <page.h>

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

#if 0

#define MBR_SECTOR_NR      1
#define LOADER_SECTOR_NR  32
#define KERNEL_SECTOR_NR 128
#define LOGO_SECTOR_NR    56

#endif


#if 0

    EIP = 56	/* EIP save at kernel stack in offset 56 */
    ESP = 68	/* ESP save at kernel stack in offset 44 */    
.align 4
_syscall_execve:
	pushl %ebx 	/* EBX point to the param file_name */
	lea EIP(%esp),%eax
	pushl %eax  /* EIP push as param for call syscall_execve */
	lea ESP(%esp),%eax
	pushl %eax  /* ESP push as param for call syscall_execve */
	call syscall_execve
	addl $12,%esp
	ret


#endif

extern int __page_map ( uint32 pgd, uint32 pa, uint32 va, int attr );

int syscall_execve( uint32 *esp, uint32 *eip,const uint8 *file_name)
 {
	// 进程在fork()时就已经具备独立的内核栈与用户栈.
	// 调用do_execve()时将两个堆栈指针恢复到栈顶.
	// 在调用execve时，压栈的地址为返回地址即EIP，当进入_syscall_execve后，EIP在
	// 内核栈的偏移位置为56字节.因此在_syscall_execve中,通过指令lea EIP(%esp),%eax 也即lea 56(%esp),%eax
	// 将返回地址存放在%eax寄存器中,然后将其压入syscall_execve()的栈帧内，得到的参数便是eip
	// 通知指针eip可以直接修改内核栈上对应EIP位置上的值.
	// 我们正是通过修改内核栈保存的EIP值达到切换系统调用返回后，直接执行新进程指令的目的.
	// 使用新程序的入口entry填入eip

	uint32 offset;

	// 从RAM_DISK内存段中载入新进程的代码.先这样做吧，后续实现文件系统以后，改为从文件系统中读取可执行文件.
	// 此处新进程由loader加载在主内存临时区8M附近.

	// 改为COW 2019.07.11
 	offset = 0x800000; // 程序入口在RAM_DISK偏移0处

	// printk("execve(): file_name=%s, ",file_name);
	// printk("EntryPoint = %08x [Instruction=%08x], SP=%08x\n", 0x2000000, *((uint32*)offset), 0x4000000);

	*eip = 0x2000000;
	*esp = 0x4000000+PAGE_SIZE-4;
	
	return (0);
}






