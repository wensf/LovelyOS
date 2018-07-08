
#-----------------------------------------------------------------------------------------------
# File Makefile for all the sources file to make the objects.
# 2016.04.18
#-----------------------------------------------------------------------------------------------
AS				= as
CC 				= gcc
LD 				= ld
OBJCOPY 		= objcopy
OBJDUMP			= objdump
CFLAGS 			= -Wall -g -I ../../include -I ../../include/lib -I ../../include/kernel -I ../../include/mm -fno-stack-protector -fno-builtin -nostdinc
LDFLAGS 		= -Ttext=0x0100 -Map loader.map
# --verbose

LOADER_BIN 		= loader.bin

SRCS_LOADER = $(wildcard *.s)
SRCC_LOADER = $(wildcard *.c)

OBJS_LOADER = $(patsubst %.s,%.o, $(SRCS_LOADER))
OBJC_LOADER = $(patsubst %.c,%.o, $(SRCC_LOADER))

Debug : $(OBJS_LOADER) $(OBJC_LOADER)
	$(LD) $(LDFLAGS) $(OBJS_LOADER) $(OBJC_LOADER) -o loader.elf
	$(OBJCOPY) -S --only-section .text --only-section .rdata --only-section .data -O binary loader.elf $(LOADER_BIN)
	$(OBJDUMP) -D loader.elf > loader.dis
loader.o:loader.s
	as -g -o loader.o loader.s

cleanDebug:
	rm *.bin *.elf *.o *.map *.dis
#---------------------------------------------------------------------------------
