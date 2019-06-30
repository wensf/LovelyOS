
#-----------------------------------------------------------------------------------------------
# File Makefile for all the sources file to make the objects.
# 2016.04.18
#-----------------------------------------------------------------------------------------------
AS				= as
CC 				= gcc
LD 				= ld
OBJCOPY 		= objcopy
OBJDUMP			= objdump
ASFLAGS         	= -W -g
CFLAGS 			= -Wall -g -I ./ -fno-stack-protector -fno-builtin -nostdinc
LDFLAGS 		= -Ttext=0x0100 -Map loader.map
# --verbose

LOADER_BIN 		= loader.bin

SRCS_LOADER = $(wildcard *.S)
SRCC_LOADER = $(wildcard *.c)

OBJS_LOADER = $(patsubst %.S,%.o, $(SRCS_LOADER))
OBJC_LOADER = $(patsubst %.c,%.o, $(SRCC_LOADER))

Debug : $(OBJS_LOADER) $(OBJC_LOADER)
	$(LD) $(LDFLAGS) $(OBJS_LOADER) $(OBJC_LOADER) -o loader.elf
	$(OBJCOPY) -S --only-section .text --only-section .rodata --only-section .data -O binary loader.elf $(LOADER_BIN)
	$(OBJDUMP) -D  -m i8086 loader.elf > loader.dis
$(OBJS_LOADER):$(SRCS_LOADER)
	$(AS) $(ASFLAGS) $< -o $@

cleanDebug:
	rm *.bin *.elf *.o *.map *.dis
#---------------------------------------------------------------------------------
