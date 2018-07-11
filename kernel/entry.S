.code32
.text

.global int3_trap_c
.global int3_trap


.align 4
int3_trap:
	push %eax
	push %ecx
	push %edx

	call int3_trap_c

	pop  %edx
	pop  %ecx
	pop  %eax
	iret
