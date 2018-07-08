/**
 * os/kernel/system_call.s
 *
 * (C) 2016 Wen Shifang
 */

/**
 * system_call.s contains the system-call low-level handling routines.
 */
.code32
.text

.global system_call
.global sys_call_table
.global sys_call0_fork
.global sys_call1_pause
.global ret_from_fork
.global do_fork
.global find_empty_process
.global schedule

.align 4
bad_sys_call:
	mov $-1, %eax
	iret

.align 4
system_call:
    cmpl $0x1,%eax
	ja   bad_sys_call
	pushl %es
	pushl %ds
	pushl %ebp
	pushl %edi
	pushl %esi
	pushl %edx  # push %ebx,%ecx,%edx as parameters
	pushl %ecx  # to system call
	pushl %ebx
#	pushl %eax
	movl  $0x10, %edx # set up ds,es,fs to kernel space
	mov   %dx, %ds
	mov   %dx, %es
	mov   %dx, %fs

	call *sys_call_table(,%eax,4)

#	pop   %ecx
	pushl %eax #(Save the %EAX at top )
#	call  schedule
ret_from_fork:
_ret_from_sys_call:
	pop   %eax
	pop   %ebx
	pop   %ecx
	pop   %edx
	pop   %esi
	pop   %edi
	pop   %ebp
	pop   %ds
	pop   %es

	iret

.align 4
sys_call0_fork:
	call find_empty_process
	testl %eax, %eax
	js 1f
	pushl %esp
	pushl %eax
	call do_fork
	addl $8, %esp
1:  ret

.align 4
sys_call1_pause:
	call schedule
	jmp _ret_from_sys_call
