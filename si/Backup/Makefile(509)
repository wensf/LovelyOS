#-----------------------------------------------------------------------------------------------
# File Makefile for all the sources file to make the objects.
# 2018.07.08
#-----------------------------------------------------------------------------------------------
AS				= as
CC 				= gcc
LD 				= ld
OBJCOPY 		= objcopy
OBJDUMP			= objdump
CFLAGS 			= -Wall -g -I ../../include -I ../../include/lib -I ../../include/kernel -I ../../include/mm -fno-stack-protector -fno-builtin -nostdinc
LDFLAGS 		= -Ttext=0x7C00 -Map boot.map
# --verbose

BOOT_BIN 		= boot.bin

SRCS_BOOT = $(wildcard *.s)

OBJS_BOOT = $(patsubst %.s,%.o, $(SRCS_BOOT))

Debug : $(OBJS_BOOT)
	$(LD) $(LDFLAGS) boot.o -s -S -o boot.elf
	$(OBJCOPY) -S --only-section .text --only-section .rodata --only-section .data -O binary boot.elf $(BOOT_BIN)
	$(OBJDUMP) -D boot.elf > boot.dis
boot.o:boot.s
	as -g -o boot.o boot.s

cleanDebug:
	rm *.bin *.elf *.o *.dis *.map
#---------------------------------------------------------------------------------
