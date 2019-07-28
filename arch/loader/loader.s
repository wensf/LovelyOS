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
 * Last modified : 2018.7.18
 */
.code16gcc				/* Assembler instruction for gas */
.data				/* data segment stay with code segment ^_^ */
.text
/**
 * Program Entry Point : 0x0900:0100
 */
.global main
	jmp		_start

.equ	MessageLength, 17
.global LoadMessage
/*.type LoadMessage, @object */
/*    .def LoadMessage; .scl 2; .type 16;
    .endef */
LoadMessage:
	.ascii "Loading kernel..."
	.ascii "Ready.           "
	.ascii "Loading failed   "
	.ascii "4GB Access failed"
	.ascii "Loading ramfs... "
	/* .size LoadMessage, . - LoadMessage */

.global DispStrRealMode /* void DispStrRealMode(short x, short y, short string_idx) */
	/*.type DispStrRealMode, @function */
	/*..def DispStrRealMode; .scl 2; .type 16;
	.endef */
DispStrRealMode:
	push    %bp
	mov     %sp, %bp
	push    %bx
	push    %di
	push    %si
	
	movw    $MessageLength, %ax
	movb    0x6(%bp),%dh
	mul     %dh
	movb 	0x8(%bp), %dh   /* (dl,dh)->(x,y) */ 
	movb    0xa(%bp), %dl	
	push    %bp
	addw    $LoadMessage, %ax
	movw    %ax, %bp
	movw	%ds, %ax
	movw 	%ax, %es
	movw	$MessageLength, %cx
	movw	$0x1301, %ax
	movw	$0x000C, %bx
	int     $0x10
	pop    	%bp
	
	pop     %si
	pop     %di
	pop     %bx	
	mov     %bp, %sp
	pop     %bp
	
	ret

	/* .size DispStrRealMode, . - DispStrRealMode */

error:
	call    DispStrRealMode
	jmp     .
	
/**
 * LBA address packet information for extends INT 0x13
 * We load 0x7f sectors form 0x21 sector to ram address
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
		.word 0x7f
	bufoffset:
		.word 0x0000
	bufseg:
		.word 0x4000
	blockNum:
	    .quad 0x0021   /* read from sector number 33 */
	/*.size packet, .-packet */	

/**
 * Load the kernel.bin from disk using extends BIOS int 0x13 function
 * (LBA mode).
 */
.global load_kernel
	/*.type load_kernel, @function */
	/*.def load_kernel; .scl 2; .type 16;
	.endef */
load_kernel:

	push    %bp
	mov     %sp, %bp	
	push    %bx
	push    %di
	push    %si

	mov     $0x80, %dl
	mov     $0x42, %ah
	mov     $packet, %si
	int     $0x13

	jc      load_kernel_int13

done:
	pushw   $0 /* x */
	pushw   $5 /* y */
	pushw   $1 /* message index */
	call	DispStrRealMode
	add     $6, %esp

	pop     %si
	pop     %di
	pop     %bx
	mov     %bp, %sp
	pop     %bp	

	ret


.global load_kernel_int13
load_kernel_int13:
	push	%bx
	push	%di
	push	%si
	push	%bp
	mov		%sp, %bp

	mov     $0x02, %ah	/* BIOS function number */
	mov     $0x20, %al	/* number of sector to read (80 secotors)*/
	mov     $0x00, %ch	/* number of cylinder */
	mov     $0x21, %cl	/* number of start secotr (from 1)*/
	mov     $0x00, %dh	/* number of header */
	mov     $0x80, %dl	/* number of driver */
	int     $0x13		/* call BIOS INT 13H*/

	jc      error

	mov     %bp, %sp
	pop     %bp
	pop     %si
	pop     %di
	pop     %bx

	jmp		done


	/* .size load_kernel, .-load_kernel */
	
.global ramfs_packet
	/*.type ramfs_packet, @object */
	/*.def packet; .scl 2; .type 16;
	.endef*/
	ramfs_packet:
	ramfs_size:
		.byte 0x10
	ramfs_reserved:
		.byte 0x00
	ramfs_count:
		.word 0x7f
	ramfs_bufoffset:
		.word 0x0000
	ramfs_bufseg:
		.word 0x4000
	ramfs_blockNum:
	    .quad 0x00A1   /* read from sector number 161 */
	/*.size packet, .-packet */		

.global show_error

.global load_ramfs	
	/*.type load_ramfs, @function */
	/*.def load_ramfs; .scl 2; .type 16;
	.endef */
load_ramfs:

	push    %bp
	mov     %sp, %bp	
	push    %bx
	push    %di
	push    %si

	mov     $0x80, %dl
	mov     $0x42, %ah
	mov     $ramfs_packet, %si
	int     $0x13


//	jc      error
	jc		show_err
	
	mov     $0x0, %esi
	mov     $0x700000, %edi
	mov     $64*1024, %ecx
read:	
	movl    %es:(%esi),%eax
	movl    %eax, %fs:(%edi)
	addl    $4, %esi
	addl    $4, %edi
	subl    $4, %ecx
	jnz     read


	pushw   $0 /* x */
	pushw   $3 /* y */
	pushw   $1 /* message index */
	call	DispStrRealMode
	add     $6, %esp

	pop     %si
	pop     %di
	pop     %bx
	mov     %bp, %sp
	pop     %bp	

	ret

show_err:
	push  %eax
	call   show_error
	jmp	   .
	
	/* .size load_ramfs, .-load_ramfs */
	
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
	mov     %ax, %ss

	/* set cpu to protected mode */
	call	entry_protected_mode_temporary
	call    access_4gb_ram_test
	
	call    main

	/* show a message : Loading ramfs... */
	pushw   $0 /* x */
	pushw   $2 /* y */
	pushw   $4 /* index in  string array */
	call	DispStrRealMode
	addw    $0x6, %sp
	
	/* load the fatfs to 0x00700000(7MB) as ramfs */
	mov		$0x4000, %ax
	mov		%ax, %es
	xor		%bx, %bx
	call 	load_ramfs

	/* show a message : Loading kernel... */
	pushw   $0 /* x */
	pushw   $4 /* y */
	pushw   $0 /* index in  string array */
	call	DispStrRealMode
	addw    $0x6, %sp

	/* load the kernel to 0x4000:0x0000 */
	mov		$0x4000, %ax
	mov		%ax, %es
	xor		%bx, %bx
	call	load_kernel

	/* let gs point to the display-ram address */
	movw	$0xB800, %ax
	movw	%ax, %gs

	/* set video to graphics mode */

	call	set_video

	jmp     _Entry_protected_mode

	/* .size _start, . - _start */
	
.global access_4gb_ram_test
access_4gb_ram_test:
	mov		$0x800000, %eax
	movl    $0x55aa,%fs:(%eax)
	movl    %fs:(%eax),%ebx
	cmpl    $0x55aa,%ebx
	jne     die
	ret
die:
	pushw   $0 /* x */
	pushw   $3 /* y */
	pushw   $3 /* index in  string array */
	call	DispStrRealMode
	addw    $0x6, %sp
	cli
	hlt	

.global entry_protected_mode_temporary
	/** entry protected mode temporary for access 4GB line space */
	
entry_protected_mode_temporary:

	/* Disalbe the interrupt and set GDTR LDTR */

	cli
	
	/* Set GDTR, the IDT not used */
	lgdt	gdt_48

	/* Enable A20 */

	inb		$0x92, %al
	orb		$0x02, %al
	outb	%al, $0x92

	/* Entry 32Bit protected mode */
	mov		%cr0, %eax
	or		$0x1, %eax
	mov		%eax, %cr0

	/* make fs point to 4BG data segment */
	mov     $0x10, %eax
	mov     %ax, %fs
				
	/* Disable A20 */
	inb     $0x92, %al
	andb    $11111110, %al
	outb    %al, $0x92
	
	/* Back to 16Bit real-mode */
	mov     %cr0, %eax
	andl    $0xFFFFFFE,%eax
	mov     %eax, %cr0	
	
	ret

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
	xor     %eax, %eax
	mov     $0x10,%ax
	mov     %ax, %ds
	mov     %ax, %es
	mov     %ax, %ss
	mov     %ax, %fs
	mov     %ax, %gs

	movl	$0x00040000, %esp

	/**
	 * Now we can address at 4GB line space, so we should
	 * relocate the kernel image to 0x00108000 from 0x00040000
	 */
	mov     $0x0, %ax
	cld
kernel_move:		/* ds:si --> es:di size-->64KB */
					/* 0x10:00040000 -> 0x10:0x00106000 */
	mov     $0x00040000, %esi
	mov     $0x00106000, %edi
	mov     $64*1024, %ecx
	rep
	movsl
	/* Do a simple check */
	movl    $64*1024, %ecx
	movl    $0x040000, %esi
	movl    $0x106000, %edi

loop:
	mov     (%esi), %eax
	mov     (%edi), %ebx
	cmp		%eax, %ebx
	jne     _not_equal
	add     $4, %esi
	add     $4, %edi
	sub     $4, %ecx	
	jnz     loop
	jmp		_equal

_equal:
	pushl	$'E'
	pushl	$1920/2
	pushl	$1080/2
	call	_putc
	add		$12, %esp

	/**
	 * Ok, we will entry 32-bit protected mode
	 * so, say good bye to real-mode
	 */
	jmpl	$0x08, $0x00106000

_not_equal:

	pushl	$'N'
	pushl	$11
	pushl	$0
	call	_putc

	ret

/* 
 * we has set the video to graphics mode, so the _putc don't work,
 * just keep it as it is.
 */
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
	add  $0xe0000000, %eax
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
