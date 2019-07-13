/**
 * os/fs/exec.c
 * Author : Wen Shifang
 * 2019.07.02
 */

#include <types.h>

/**
 *
 * do_execve() to execute a new program.
 */

int do_execve( uint32 *eip, const uint8 *file_name )
{
	// 进程在fork()时就已经具备独立的内核栈与用户栈.
	// 调用do_execve()时将两个堆栈指针恢复到栈顶
	// 
	// 使用新程序的入口entry填入eip

	current->thread.


	return (0);
}

