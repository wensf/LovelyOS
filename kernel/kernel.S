.code32
.data
.text

.global main
.global _start
.global task_idle_user_stk

.global _idt
.global _gdt

_start:
	movw	$0x10, %ax
	mov	%ax, %ds
	mov	%ax, %es
	mov	%ax, %ss
	mov	%ax, %fs
	mov	%ax, %gs	

_clear_bss:
	xorl %eax,%eax
	movl $__bss_start,%edi
	movl $__bss_end,%ecx
	subl %edi,%ecx
	cld
	rep
	stosb

	lidt	idt_48
	lgdt	gdt_48

	jmpl    $0x08, $_flush
_flush:
	movw	$0x10, %ax
	mov		%ax, %ds
	mov		%ax, %es
	mov		%ax, %ss
	mov		%ax, %fs
	mov		%ax, %gs	

	movl $task_idle_user_stk, %eax
	addl $0x1000*4-4, %eax
	movl %eax, %esp
	
	jmpl	$0x08, $main

.align 		4
_idt:
	.fill	256, 8 , 0	/* IDT is uninitialized */
_gdt:
	.word	0,0,0,0		/* dummy */
_cs_kernel:
	.word	0xFFFF		/* 4Gb -limit=65536*16 (65536*16*4096=4GB) */
	.word   0x0000		/* Base address = 0 */
	.word   0x9800		/* Code exec only, P=1,S=1,DPL=0,D/B=1 */
	.word	0x00CF 		/* granularity = 4096, 386 */
_ds_kernel:
	.word	0xFFFF		/* 4Gb -limit=65536*16 (65536*16*4096=4GB) */
	.word   0x0000		/* Base address = 0 */
	.word   0x9200		/* Data read/write, P=1,S=1,DPL=0,D/B=1 */
	.word	0x00CF 		/* granularity = 4096, 386 */
_cs_user:
	.word   0xFFFF		/* 4GB -limit=65536*16(65536*16*4096=4GB) */
	.word   0x0000      /* Base address = 0 */
	.word   0xF800      /* Code exce only,P=1,S=1,DPL=3,D/B=1 */
	.word   0x00CF
_ds_user:
	.word   0xFFFF		/* 4GB -limit=65536*16(65536*16*4096=4GB) */
	.word   0x0000      /* Base address = 0 */
	.word   0xF200      /* Date read/write,P=1,S=1,DLP=3,D/B=1 */
	.word   0x00CF      /* granularity = 4096, 386 */

	.word	0,0,0,0		/* dummy */
	.fill	250, 8, 0
idt_48:
	.word	0x0800		/* IDT size (256 * 8)*/
	.long	_idt		/* IDT base address */
gdt_48:
	.word	0x0800		/* GDT size (256 * 8)*/
	.long	_gdt		/* GDT base address */	
	
