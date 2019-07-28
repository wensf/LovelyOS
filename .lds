OUTPUT_FORMAT("pe-i386","pe-i386","pe32-i386")
OUTPUT_ARCH(i386)
ENTRY(_start)

SECTIONS{

	. = 0xC0108000;
	
	. = ALIGN(4096);
	__code_start = .;
	.text : {
		kernel/kernel.o
		init/main.o	(.text)
		*.o		(.text)
	}

	. = ALIGN(4096);
	.rodata : {
		*(.rodata)
	}
	__code_end = .;

	. = ALIGN(4096);
	__data_start = .;
	.data : {
		*(.data)
	}
	__data_end = .;

	. = ALIGN(4096);
	__bss_start = .;
	.bss : {
		*(.bss)
	}
	__bss_end = .;
}


