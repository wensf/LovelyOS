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
.global nr_sys_calls

.align 4
bad_sys_call:
	mov $-1, %eax
	iret
                    /**
                     * System call entry by int 0x80 instruction
                     *
                     */
.align 4
system_call:
    cmpl $nr_sys_calls,%eax
	ja   bad_sys_call
	push  %es               /**/
	push  %ds                /*  eip,cs eflags, esp, ss */
	push  %gs                /*  save by CPU. */
	push  %fs                /*  we need to save the ds,es,gs,fs, */
	pushl %ebp               /*  ebp,edi,esi, edx, ecx,ebx*/
	pushl %edi               /*  Note: eax don't need to save */
	pushl %esi               /*  it just use to pass the system call nr */
	pushl %edx               /**/
	pushl %ecx
	pushl %ebx

	movl  $0x10, %edx
	mov   %dx, %ds
	mov   %dx, %es
	mov   %dx, %fs
	mov   %dx, %gs

	call  *sys_call_table(,%eax,4)

_ret_from_sys_call:
	popl  %ebx
	popl  %ecx
	popl  %edx
	popl  %esi
	popl  %edi
	popl  %ebp
	pop   %fs
	pop   %gs
	pop   %es
	pop   %ds
	iret

ret_from_fork:
	popl  %eax
	popl  %ebx
	popl  %ecx
	popl  %edx
	popl  %esi
	popl  %edi
	popl  %ebp
	pop   %fs
	pop   %gs
	pop   %es
	pop   %ds
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
    ret


.global timer_int
.global timer_interrupt

.align 4
timer_int:
	pushl %eax
	pushl %ecx
	pushl %edx

	movb $0x20, %al
	outb %al, $0x20

	call timer_interrupt

	popl  %edx
	popl  %ecx
	popl  %eax
	iret
