OUTPUT_FORMAT("pe-i386","pe-i386","pe32-i386")
OUTPUT_ARCH(i386)
ENTRY(_start)

SECTIONS{
	. = 0x00000000;
	__start = .;
	
	. = 0x00108000;
	. = ALIGN(4096);
	__code_start = .;
	.text : {
		kernel/kernel.o	(.text)
		init/main.o		(.text)
		*.o				(.text)
	}
	. = ALIGN(4096);
	.rdata : {
		*(.rodata)
	}
	. = ALIGN(4096);
	__code_end = .;

	. = ALIGN(4096);
	__data_start = .;
	.data : {
		*(.data)
	}
	. = ALIGN(4096);
	__data_end = .;

	. = ALIGN(4096);
	__bss_start = .;
	.bss : {
		*(.bss)
	}
	. = ALIGN(4096);
	__bss_end = .;
	__end = .;
}
