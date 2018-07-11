.global putc	/* void putc(int x, int y, char c) */

putc:
	push %ebp
	mov  %esp, %ebp
	push %eax
	push %ebx

	xor  %eax, %eax
	mov  $80, %eax
	movl 12(%ebp), %ebx  /* parmater y */
	mul  %ebx
	add  8(%ebp), %eax   /* parmater x */
	mov  $0x2, %ebx
	mul  %ebx
	add  $0xB8000, %eax
	mov  %eax, %ebx
	xor  %eax, %eax
	mov  16(%ebp), %eax  /* parmater c */
	and  $0xFF, %eax
	orl  $0x0000C00, %eax
	movw %ax, (%ebx)

	pop  %ebx
	pop  %eax
	pop  %ebp

	ret
