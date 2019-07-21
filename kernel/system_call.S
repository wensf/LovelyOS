/**
 * os/kernel/system_call.s
 *
 * (C) 2016 Wen Shifang
 *
 * Create 2016.5.20
 * Modify 2018.7.29
 */

/**
 * system_call.S contains the system-call low-level handling routines.
 */
.code32
.text

.global system_call
.global sys_call_table
.global syscall_fork
.global _syscall_fork
.global syscall_pause
.global _syscall_pause
.global syscall_sleep
.global _syscall_sleep
.global ret_from_fork
.global do_fork
.global schedule
.global syscall_open  	/* int _syscall_open( const char *file_name, int mode, int flag ); */
.global _syscall_open
.global syscall_read  	/* int _syscall_read( int fd, char *__buf, int cnt ); */
.global _syscall_read
.global syscall_write 	/* int syscall_write ( int fd, const char *__buf, int len ) in write.c */
.global _syscall_write
.global _syscall_close	/* extern int _syscall_close( int fd ); */
.global syscall_close
.global _syscall_dup	/* extern int _syscall_dup( int fd ); */
.global syscall_dup
.global _syscall_lseek   /* int syscall_lseek( int fd, int offset, int whence ); */
.global syscall_lseek
.global _syscall_idle   /* int syscall_idle(void); */
.global syscall_idle
.global _syscall_get_utime /* unsigned int get_utime(void); */
.global syscall_get_utime
.global _syscall_get_ktime /* unsigned int get_ktime(void); */
.global syscall_get_ktime
.global _syscall_execve
.global syscall_execve		/* int syscall_execve(const char *file_name); */
.global _syscall_get_pid
.global syscall_get_pid		/* int syscall_get_pid(void); */
.global _syscall_mmap		
.global sys_mmap			/* unsigned int _syscall_mmap(unsigned long start, unsigned long length, unsigned long flags); */
.global _syscall_exit
.global syscall_exit		/* void syscall_exit(int exit_code);  */
.global _syscall_wait
.global syscall_wait		/* int syscall_wait(int *exit_code);  */
.global _syscall_idle
.global syscall_idle		/* void syscall_idle(void);  */
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
	pushl  $0                 /* as a dummy 'error code' */
	push  %ds                /**/
	push  %es                /*  eip,cs eflags, esp, ss saved by CPU.*/
	push  %gs                 
	push  %fs                /*  we need to save the ds,es,gs,fs, */
	pushl %ebp               /*  ebp,edi,esi, edx, ecx,ebx*/
	pushl %edi               /*  Note: eax don't need to save */
	pushl %esi               /*  it just use to pass the system call nr */
	pushl %edx               /**/
	pushl %ecx
	pushl %ebx
	pushl %eax

	pushl %edx
	movl  $0x10, %edx		/* Ring0 Code Segment */
	mov   %dx, %ds
	mov   %dx, %es
	mov   %dx, %gs
	movl  $0x23, %edx		/* FS set to 0x23 for access user space */
	mov   %dx, %fs
	popl  %edx

	call  *sys_call_table(,%eax,4)

	addl  $4, %esp
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
	addl  $4, %esp 	        /* pop a dummy 'error code' */
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
	addl  $4, %esp 	        /* pop a dummy 'error code' */
	iret

.align 4
_syscall_fork:
	pushl %esp
	call do_fork
	addl $4, %esp
	pushl %eax
	call schedule
	popl %eax
1:  ret

.align 4
_syscall_pause:
    call syscall_pause
	call schedule
    ret

.align 4
_syscall_sleep:
    pushl %ebx
    call syscall_sleep
    addl  $4, %esp
	call schedule
    ret

.align 4
_syscall_open:
    pushl %edx
	pushl %ecx
	pushl %ebx
    call syscall_open
    addl  $12, %esp
    //pushl %eax  // save the result    
	//call schedule
	//popl %eax	
    ret

.align 4
_syscall_read:
    pushl %edx
	pushl %ecx
	pushl %ebx
    call syscall_read
    addl  $12, %esp
//	call schedule
    ret

.align 4
_syscall_write:
    pushl %edx
	pushl %ecx
	pushl %ebx
    call syscall_write
    addl  $12, %esp
 //   pushl %eax  // save the result
//	call schedule
//	popl %eax
    ret

.align 4
_syscall_close:
    pushl %ebx
    call syscall_close
    addl  $4, %esp
//	call schedule
    ret


.align 4
_syscall_dup:
    pushl %ebx
    call syscall_dup
    addl  $4, %esp
//	call schedule
    ret

.align 4
_syscall_lseek:
    pushl %edx
	pushl %ecx
	pushl %ebx
    call syscall_lseek
    addl  $12, %esp
//	call schedule
    ret

.align 4
_syscall_idle:
    hlt
    call syscall_idle
	call schedule
    ret

.align 4
_syscall_get_utime:
    call syscall_get_utime
    ret

.align 4
_syscall_get_ktime:
    call syscall_get_ktime
    ret


    EIP = 56	/* EIP save at kernel stack in offset 56 */
    ESP = 72	/* ESP save at kernel stack in offset 72 */    
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

.align 4
_syscall_get_pid:
    call syscall_get_pid
    ret	


.align 4
_syscall_mmap:
    pushl %edx
	pushl %ecx
	pushl %ebx
    call syscall_mmap
    addl  $12, %esp
    ret
    

.align 4
_syscall_exit:
	pushl %ebx
    call syscall_exit
    addl  $4, %esp
    ret

.align 4
_syscall_wait:
	pushl %ebx
    call syscall_wait
    addl  $4, %esp
    ret

.global _parallel_interrupt
.global parallel_interrupt
.align  4
_parallel_interrupt:
	pushl $0
	push %ds
	push %es
	push %gs
	push %fs
	pushl %ebp
	pushl %edi
	pushl %esi
	pushl %edx
	pushl %ecx
	pushl %ebx
	pushl %eax

    pushl %esp

	movb $0x20, %al
	outb %al, $0x20

    movl  44(%esp), %eax
    andl  $3, %eax            # cpl
    pushl %eax
    movl $0x10,%edx
    mov   %dx,%ds
    mov   %dx,%es
    mov   %dx,%fs
    mov   %dx,%gs

	call parallel_interrupt
    addl  $8,%esp

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
    addl  $4, %esp
    iret

.global timer_int
.global timer_interrupt

.align 4
timer_int:
	pushl $0
	push %ds
	push %es
	push %gs
	push %fs
	pushl %ebp
	pushl %edi
	pushl %esi
	pushl %edx
	pushl %ecx
	pushl %ebx
	pushl %eax

    pushl %esp

	movb $0x20, %al
	outb %al, $0x20

    movl  44(%esp), %eax
    andl  $3, %eax            # cpl
    pushl %eax
    movl $0x10,%edx
    mov   %dx,%ds
    mov   %dx,%es
    mov   %dx,%fs
    mov   %dx,%gs

	call timer_interrupt
    addl  $8,%esp

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
    addl  $4, %esp
    iret



.global _keyboard_irq
.global keyboard_irq

.align 4
_keyboard_irq:
	pushl $0
	push %ds
	push %es
	push %gs
	push %fs
	pushl %ebp
	pushl %edi
	pushl %esi
	pushl %edx
	pushl %ecx
	pushl %ebx
	pushl %eax

    pushl %esp

	movb $0x20, %al
	outb %al, $0x20

    movl  44(%esp), %eax
    andl  $3, %eax            # cpl
    pushl %eax
    movl $0x10,%edx
    mov   %dx,%ds
    mov   %dx,%es
    mov   %dx,%fs
    mov   %dx,%gs

	call keyboard_irq
    addl  $8,%esp

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
    addl  $4, %esp
    iret

.global _generic_protected_fault
.global generic_protected_fault

.align 4
_generic_protected_fault:
	/* Error Code push by CPU auto */
	push %ds
	push %es
	push %gs
	push %fs
	pushl %ebp
	pushl %edi
	pushl %esi
	pushl %edx
	pushl %ecx
	pushl %ebx
	pushl %eax

    pushl %esp

    movl  44(%esp), %eax                #
    andl  $3, %eax                      # cpl
    pushl %eax
    movl $0x10,%edx
    mov   %dx,%ds
    mov   %dx,%es
    mov   %dx,%fs
    mov   %dx,%gs

	call generic_protected_fault
    addl  $8,%esp

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
    addl  $4, %esp
    iret


.global _int3_trap
.global int3_trap

.align 4
_int3_trap:
	pushl $0                       /** No Error Code */
	push %ds
	push %es
	push %gs
	push %fs
	pushl %ebp
	pushl %edi
	pushl %esi
	pushl %edx
	pushl %ecx
	pushl %ebx
	pushl %eax

    pushl %esp

    movl  44(%esp), %eax                #
    andl  $3, %eax                      # cpl
    pushl %eax
    movl $0x10,%edx
    mov   %dx,%ds
    mov   %dx,%es
    mov   %dx,%fs
    mov   %dx,%gs

	call int3_trap
    addl  $8,%esp

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
    addl  $4, %esp
    iret

.global _operation_fault
.global operation_fault

.align 4
_operation_fault:
	pushl $0                       /** No Error Code */
	push %ds
	push %es
	push %gs
	push %fs
	pushl %ebp
	pushl %edi
	pushl %esi
	pushl %edx
	pushl %ecx
	pushl %ebx
	pushl %eax

    pushl %esp

    movl  44(%esp), %eax                #
    andl  $3, %eax                      # cpl
    pushl %eax
    movl $0x10,%edx
    mov   %dx,%ds
    mov   %dx,%es
    mov   %dx,%fs
    mov   %dx,%gs

	call operation_fault
    addl  $8,%esp

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
    addl  $4, %esp
    iret    


.global _page_fault
.global page_fault
.global do_no_page
.global do_wp_page

.align 4
_page_fault:
	xchgl %eax,(%esp)				
	pushl %ecx
	pushl %edx
	push %ds
	push %es
	push %fs
	movl $0x10, %edx
	mov  %dx, %ds
	mov  %dx, %es
	mov  %dx, %fs
	movl %cr2, %edx
	push %edx
	push %eax
	testl $1, %eax
	jne  1f
	call do_no_page
	jmp	 2f
1:	call do_wp_page
2:	addl $8, %esp
	pop  %fs
	pop  %es
	pop  %ds
	popl %edx
	popl %ecx
	popl %eax
	iret


	
