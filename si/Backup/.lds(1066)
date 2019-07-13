OUTPUT_FORMAT("pe-i386","pe-i386","pe32-i386")
OUTPUT_ARCH(i386)
ENTRY(_start)

SECTIONS{

	. = 0x00108000;
	. = ALIGN(4);
	.text : {
		kernel/kernel.o	(.text)
		init/main.o		(.text)
		*.o				(.text)
	}

	. = ALIGN(4);
	.rdata : {
		*(.rdata)
	}

	. = ALIGN(4);
	.data : {
		*(.data)
	}

	. = ALIGN(4);
	.bss : {
		*(.bss)
	}

}
