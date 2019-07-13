/**
 * This program will linked at 0x7C00 and the BIOS will
 * load it into memory address 0x0000:0x7C00.
 * 2016.4.24
 */
	.code16				/* Assembler instructino for gas */
 	.text
	.global _start

_start:							/* entry point */
	movw	%cs, %ax
	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %ss
	movw	%ax, %gs
	movw	%ax, %fs
	movw	$0x7c00, %sp

	/* clear screen */
	mov     $0x0600, %ax
	mov	    $0x0000, %cx
	movb    $0x24, %dh
	movb    $0x79, %dl
	movb	$0x07, %bh
	int     $0x10

	/* set cur (x,y)->(0x0,0x0) */
	movb    $0x02, %ah
	movb    $0x00, %bh
	movb	$0x00, %dh
	movb	$0x00, %dl
	int		$0x10

	/* show a message Booting... */
	movb	$0, %dh
	call	Disptr

	/* reset disk driver */
	xor		%ah, %ah
	xor		%dl, %dl
	int		$0x13

	/* load the loader from disk to memory 0x9000:0x0100 */
	mov		$0x9000, %ax /* buffer : es:bx*/
	mov		%ax, %es
	mov     $0x0100, %bx

	call	read_loader

	/* show a message */
	movb	$1, %dh
	call	Disptr

	/* Ok, we go to 0x9000:0x0100 */

	ljmp	$0x9000, $0x0100

/**
 * to show a message
 */
Disptr:
	movw	$MessageLength, %ax
	mul     %dh
	add     $BootMessage, %ax
	mov     %ax, %bp
	mov     %ds, %ax
	mov     %ax, %es
	mov     $MessageLength, %cx
	mov     $0x1301, %ax
	mov     $0x000C, %bx
	mov     $0x0, %dl
	/* mov  $0x3, %dh  */
	int     $0x10
	ret

/**
 * Read 4 sectors data from disk by BIOS INT13 function (CHS mode)
 * we place the data at 0x9000:0x0100
 */

read_loader:
	push	%bx
	push	%di
	push	%si
	push	%bp
	mov		%sp, %bp

	mov     $0x02, %ah	/* BIOS function number */
	mov     $0x20, %al	/* number of sector to read (32 secotors)*/
	mov     $0x00, %ch	/* number of trunk */
	mov     $0x02, %cl	/* number of start secotr (from 1)*/
	mov     $0x00, %dh	/* number of header */
	mov     $0x80, %dl	/* number of driver */
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
	call    Disptr
	jmp     .

/**
 * constance variable define
 */
	.equ	MessageLength, 10
BootMessage:
	.ascii "Booting   "
Message:
	.ascii "Ready.    "
MessageErr:
    .ascii "Read Error"
    
/**
 * Hard disk partition table
 * 0x80: OS Active Flag
 * 0x20: start header
 * 0x21: start sector
 * 0x00: start cylinder
 * 0x04: FAT16
 * 0X25: end header
 * 0x24: end sector
 * 0x01: end cylinder
 * 0x00080000(0x00000800) offset sector of partition -> partiton start at sector 2048
 * 0x00400000(0x00004000) all sectors -> 16384 sectors this partition.
 * Note : we have 8MB fat16 parition now.
 */    
	.org  446
	.byte 0x80
	.byte 0x20
	.byte 0x21
	.byte 0x00
	.byte 0x04
	.byte 0x25
	.byte 0x24
	.byte 0x01
	.byte 0x00
	.byte 0x08
	.byte 0x00
	.byte 0x00
	.byte 0x00
	.byte 0x40
	.byte 0x00
	.byte 0x00
	
/**
 * Bootable device flage 0xAA55
 */
	.org	510
	.word	0xAA55
