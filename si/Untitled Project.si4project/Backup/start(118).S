/**
 * File : kernel.s	the kernel entry point.
 * Author : Wen Shifang
 * Created : 2016.5.1
 * Last modified : 2016.5.5
 */
.code32			/* Assembler instruction */
.data
.text

.global main
.global _start
.global _idt
.global _gdt
.global _putc
.global task_idle_user_stk
/**
 * The Kernel entry point, linked at 0x00008000 in the future.
 */

.global idt_addr

_start:				/* we just print a chacter  */
	mov		$(80*12+0x0)*2, %eax
	mov		$0xB8000, %ebx
	addl 	%eax, %ebx
	mov		$0x0C, %ah						/* 0000: black 1111: white */
	mov		$'K', %al
	mov		%ax, (%ebx)

	jmp		set_idt

set_idt:
	lea     idt_48,%eax
	subl	$0xC0000000, %eax
	lea     _idt,%ebx
	subl    $0xC0000000,%ebx
	movl    %ebx, 2(%eax)
	lidt	-0xC0000000(idt_48)
set_gdt:
	lgdt	gdt_48

//	jmpl    $0x08, $_flush
_flush:
	movw	$0x10, %ax
	mov		%ax, %ds
	mov		%ax, %es
	mov		%ax, %ss
	mov		%ax, %fs
	mov		%ax, %gs

_clear_bss:
	xorl %eax,%eax
	movl $__bss_start,%edi
	movl $__bss_end,%ecx
	subl %edi,%ecx
	cld
	rep
	stosb

	movl $task_idle_user_stk, %eax
	addl $4092, %eax
	movl %eax, %esp
	
    call    setup_paging

    jmp		.

	jmpl	$0x08, $main

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
idt_addr:
	.long	0	

/**
 * To create the page table mapping.
 * page table directory base address at 0x00100000(1MB)
 * and we use 4 pages :
 *
 * page 0 (place at 0x00101000)  0x00000000 0x00000000 --> 0- 3M 
 * page 1 (place at 0x00102000)  0x00400000 0x00400000 --> 4- 7M 
 *
 * page 2 (place at 0x00103000)  0xC0000000 0x00000000 --> 0- 3M 
 * page 3 (place at 0x00104000)  0xC0400000 0x00400000 --> 4- 7M 
 */
.global setup_paging
    /*.type setup_paging, @function */
/*    .def setup_paging; .scl 2; .type 32 */
setup_paging:
	
    movl $1024*5, %ecx
    xorl %eax, %eax
    movl $0x00010000, %edi
    cld
    rep
    stosl

    movl $0x00100000, %eax
    movl $0x00100000+0x1000+0x7,  0(%eax)
    movl $0x00100000+0x2000+0x7,  4(%eax)
    movl $0x00100000+0x3000+0x7, 3072(%eax)
    movl $0x00100000+0x4000+0x7, 3076(%eax)
    std
    movl $(0x00100000+0x4000)+4092, %edi
    movl $0x7ff007, %eax
1:  stosl
    subl $0x1000, %eax
    jge 1b
    movl $0x00100000, %eax  /* page directory at 0x00100000 */
    movl %eax, %cr3
    movl %cr0, %eax
    orl  $0x80000000, %eax
    movl %eax, %cr0
    ret

    /* .size setup_paging, . - setup_paging */
