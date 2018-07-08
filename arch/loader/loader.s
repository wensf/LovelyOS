/**
 * Loader.s, the step-2 code, it will loaded into the memory at 0x9000:0100.
 * There are something will be to do:
 * 1, load the kernel image to system ram at 0x00040000 from usb-hdd disk.
 * 2, detect onboard device and hardware information ( ram, hard disk, vga, ect)
 * 3, set temporary idt and gdt, entry 32-bit protected mode (x86 cpu)
 * 4, move the kernel image from 0x00040000 to 0x00108000
 * 5, jump to kernel entry point.
 *
 * Author : Wen Shifang
 * Created : 2016.4.25
 * Last modified : 2016.5.5
 */
.code16gcc				/* Assembler instruction for gas */
.data				/* data segment stay with code segment ^_^ */
.text
/**
 * Program Entry Point : 0x0900:0100
 */
 .global my_cfun
	jmp		_start

.equ	MessageLength, 10
.global LoadMessage
/*.type LoadMessage, @object */
/*    .def LoadMessage; .scl 2; .type 16;
    .endef */
LoadMessage:
	.ascii "Loading   "
	.ascii "Ready.    "
	.ascii "Boot error"
	/* .size LoadMessage, . - LoadMessage */

.global DispStrRealMode
	/*.type DispStrRealMode, @function */
	/*..def DispStrRealMode; .scl 2; .type 16;
	.endef */
DispStrRealMode:

	movw    $MessageLength, %ax
	mul     %dh
	addw    $LoadMessage, %ax
	movw    %ax, %bp
	movw	%ds, %ax
	movw 	%ax, %es
	movw	$MessageLength, %cx
	movw	$0x1301, %ax
	movw	$0x000C, %bx
	movb	$0, %dl
	addb	$7, %dh
	int     $0x10

	ret
	/* .size DispStrRealMode, . - DispStrRealMode */

.global load_kernel
	/*.type load_kernel, @function */
	/*..def load_kernel; .scl 2; .type 16;
	.endef*/
load_kernel:

	push	%bx
	push	%di
	push	%si
	push	%bp
	mov		%sp, %bp

	/* header 0 : read 6-18 sector (13) size ( 0x1A00 )*/
	mov     $0x02, %ah	/* BIOS function number */
	mov     $0x0D, %al	/* number of sector to read */
	mov     $0x00, %ch	/* number of trunk */
	mov     $0x06, %cl	/* number of start secotr (from 1)*/
	mov     $0x00, %dh	/* number of header */
	mov     $0x00, %dl	/* number of driver */
	int     $0x13		/* call BIOS INT 13H*/

	/* header 1 : read 1-18 sector (18) */
	mov     $0x1A00, %bx
	mov     $0x02, %ah  /* BIOS function number */
	mov     $0x12, %al	/* number of sector to read */
	mov     $0x00, %ch	/* number of trunk */
	mov     $0x01, %cl	/* number of start secotr (from 1)*/
	mov     $0x01, %dh	/* number of header */
	mov     $0x00, %dl	/* number of driver */
	int     $0x13		/* call BIOS INT 13H*/

	jc      error

	mov     %bp, %sp
	pop     %bp
	pop     %si
	pop     %di
	pop     %bx

	ret

error:
	mov     $2, %dh
	call    DispStrRealMode
	jmp     .
	/* .size load_kernel, . - load_kernel */

/**
 * Load the kernel.bin from disk using extends BIOS int 0x13 function
 * (LBA mode).
 */
.global load_kernel_ex
	/*.type load_kernel_ex, @function */
	/*.def load_kernel_ex; .scl 2; .type 16;
	.endef */
load_kernel_ex:

	push    %bx
	push    %di
	push    %si
	push    %bp
	mov     %sp, %bp

	mov     $0x80, %dl
	mov     $0x42, %ah
	mov     $packet, %si
	int     $0x13

	jc      error

	mov     %bp, %sp
	pop     %bp
	pop     %si
	pop     %di
	pop     %bx

	ret

	/* .size load_kernel_ex, .-load_kernel_ex */

.global _start
	/*.type _start, @function */
	/*.def _start; .scl 2; .type 16;
	.endef */
_start:
	/* Modify all Segement Register to 0x9000 */

	mov     %cs, %ax
	mov     %ax, %ds
	mov     %ax, %es
	mov     %ax, %gs

	call    my_cfun


	/* show a message : Loading... */
	mov     $0x0, %dh
	call	DispStrRealMode

	/* load the kernel to 0x4000:0x0000 */
	mov		$0x4000, %ax
	mov		%ax, %es
	xor		%bx, %bx
	call	load_kernel_ex

	call	KillMotor

	/* show a message : Ready... */
	movb	$1, %dh
	call	DispStrRealMode

	/* let gs point to the display-ram address */
	movw	$0xB800, %ax
	movw	%ax, %gs

	jmp     _Entry_protected_mode

	/* .size _start, . - _start */

/**
 * LBA address packet information for extends INT 0x13
 * We load 0x20 sectors form 0x05 sector to ram address
 * 0x4000:0000
 */
.global packet
	/*.type packet, @object */
	/*.def packet; .scl 2; .type 16;
	.endef*/
	packet:
	size:
		.byte 0x10
	reserved:
		.byte 0x00
	count:
		.word 0x20
	bufoffset:
		.word 0x0000
	bufseg:
		.word 0x4000
	blockNum:
	    .quad 0x0009   /* read from sector number 9 */
	/*.size packet, .-packet */

.global KillMotor
	/*.type KillMotor, @function */
	/*.def KillMotor; .scl 2; .type 16;
	.endef*/
KillMotor:
	pushw	%dx
	movw	$0x03F2, %dx
	movb	$0, %al
	out     %al, %dx
	pop     %dx

	ret
	/*.size KillMotor, . - KillMotor */

.global _Entry_protected_mode
	/*.type _Entry_protected_mode, @function */
	/*.def _Entry_protected_mode; .scl 2; .type 16;
	.endef*/
_Entry_protected_mode:

	/* Disalbe the interrupt and set GDTR LDTR */
	cli
	lidt	idt_48
	lgdt	gdt_48

	/* Enable A20 */

	inb		$0x92, %al
	orb		$0x02, %al
	outb	%al, $0x92

	/* Entry 32Bit protected mode */
	mov		%cr0, %eax
	or		$0x1, %eax
	mov		%eax, %cr0

	/* Do a long jump to refresh the code selector */
	jmpl	$0x8, $_setup32 + 0x00090000

	/* .size _Entry_protected_mode, . - _Entry_protected_mode */

	/* GDT table define temp */

_gdt:
	/* segment limited2 4Gb -limit=65536*16 (65536*16*4096=4GB) */
	/* P=1,DPL=0,S=1 TYPE=8(exec only)*/
	/* G=1(4K),D/B=1(32/8bit),AVL=0, limited2=0xF */
_dummy:
	.word	0,0,0,0	/* dummy */
_cs:
	.word	0xFFFF	/* 4Gb -limit=65536*16 (65536*16*4096=4GB) */
	.word   0x0000	/* Base address = 0 */
	.word   0x9800	/* Data read/write */
	.word	0x00CF 	/* granularity = 4096, 386 */

_ds:
	.word	0xFFFF	/* 4Gb -limit=65536*16 (65536*16*4096=4GB) */
	.word   0x0000	/* Base address = 0 */
	.word   0x9200	/* Data read/write */
	.word	0x00CF 	/* granularity = 4096, 386 */
idt_48:				/* idt not use now */
	.word	0
	.word	0,0
gdt_48:
	.word	0x0800
	.word	_gdt, 0x0009

/* That will be run as 32 bit code */
.code32
_setup32:

	/* update all segment selector register except for cs */
	xor     %ax, %ax
	mov     $0x10,%ax
	mov     %ax, %ds
	mov     %ax, %es
	mov     %ax, %ss
	mov     %ax, %fs
	mov     %ax, %gs

	movl	$0x00007C00, %esp

	/**
	 * Now we can address at 4GB line space, so we should
	 * relocate the kernel image to 0x00108000 from 0x00040000
	 */
	mov     $0x0, %ax
	cld
kernel_move:		/* ds:si --> es:di size-->64KB */
					/* 0x10:00040000 -> 0x10:0x00108000 */
	mov     $0x00040000, %esi
	mov     $0x00108000, %edi
	mov     $64*1024, %ecx
	rep
	movsl

	/* Do a simple check */
	movl	(0x040000), %eax
	movl	(0x108000), %ebx
	cmp     %ebx, %eax
	jnz     _not_equal
	cmp		$0x0, %ebx
	jz     _not_equal
	movl	(0x040004), %eax
	movl	(0x108004), %ebx
	cmp     %ebx, %eax
	jnz     _not_equal
	cmp		$0x0, %ebx
	jz     _not_equal
	movl	(0x040008), %eax
	movl	(0x108008), %ebx
	cmp     %ebx, %eax
	jnz     _not_equal
	cmp		$0x0, %ebx
	jz     _not_equal
	movl	(0x04000c), %eax
	movl	(0x10800c), %ebx
	cmp     %ebx, %eax
	jnz     _not_equal
	cmp		$0x0, %ebx
	jz     _not_equal
	movl	(0x040010), %eax
	movl	(0x108010), %ebx
	cmp     %ebx, %eax
	jnz     _not_equal
	cmp		$0x0, %ebx
	jz     _not_equal
	movl	(0x040014), %eax
	movl	(0x108014), %ebx
	cmp     %ebx, %eax
	jnz     _not_equal
	cmp		$0x0, %ebx
	jz     _not_equal
	movl	(0x040018), %eax
	movl	(0x108018), %ebx
	cmp     %ebx, %eax
	jnz     _not_equal
	cmp		$0x0, %ebx
	jz     _not_equal
	movl	(0x04001c), %eax
	movl	(0x10801c), %ebx
	cmp     %ebx, %eax
	jnz     _not_equal
	cmp		$0x0, %ebx
	jz     _not_equal

_equal:
	pushl	$'E'
	pushl	$11
	pushl	$0
	call	_putc
	add		$12, %esp

	/**
	 * Ok, we will entry 32-bit protected mode
	 * so, say good bye to real-mode
	 */

	jmpl	$0x08, $0x00108000

_not_equal:

	pushl	$'N'
	pushl	$11
	pushl	$0
	call	_putc
	jmp		.

	ret

.global _putc
_putc:
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
