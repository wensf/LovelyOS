#-----------------------------------------------------------------------------------------------
# File Makefile for all the sources file to make the objects.
# 2016.04.18
#-----------------------------------------------------------------------------------------------
AS				= as
CC 				= gcc
LD 				= ld
OBJCOPY 		= objcopy
OBJDUMP			= objdump
CFLAGS 			= -Wall -g -I include -I include/lib -I include/kernel -I include/mm -fno-stack-protector -fno-builtin -nostdinc
LDFLAGS 		= -T.lds
#LDFLAGS         = -Ttext=0x00108000
BOOT_BIN        = boot.bin
LOADER_BIN      = loader.bin
KERNEL_BIN 		= kernel.bin
IMAGE			= arch/a.img
IMAGE_MOUNT_AT	= /mnt/floppy

SRCS_KERNEL = $(wildcard kernel/*.s)
SRCC_KERNEL = $(wildcard kernel/*.c)
SRCC_INIT   = $(wildcard init/*.c)
SRCC_LIB    = $(wildcard lib/*.c)
SRCS_LIB    = $(wildcard lib/*.s)
SRCC_MM     = $(wildcard mm/*.c)

OBJS_KERNEL = $(patsubst %.s,%.o, $(SRCS_KERNEL))
OBJC_KERNEL = $(patsubst %.c,%.o, $(SRCC_KERNEL))
OBJC_INIT   = $(patsubst %.c,%.o, $(SRCC_INIT))
OBJC_LIB    = $(patsubst %.c,%.o, $(SRCC_LIB))
OBJS_LIB    = $(patsubst %.s,%.o, $(SRCS_LIB))
OBJC_MM     = $(patsubst %.c,%.o, $(SRCC_MM))

Debug : $(BOOT_BIN) $(LOADER_BIN) $(OBJS_KERNEL) $(OBJC_KERNEL) $(OBJS_LIB)  $(OBJC_LIB) $(OBJC_MM) $(OBJC_INIT)
	$(LD) $(LDFLAGS) --oformat elf32-i386 -o kernel.elf $(OBJS_KERNEL) $(OBJC_KERNEL) $(OBJS_LIB) $(OBJC_LIB)  $(OBJC_MM) $(OBJC_INIT)
	$(OBJCOPY) -S -O binary kernel.elf $(KERNEL_BIN)
	$(OBJDUMP) -D kernel.elf > kernel.dis
	mkdir -p Debug;
	cp kernel.elf Debug/os
	make image

$(BOOT_BIN):
	cd arch/; make

$(LOADER_BIN):
	cd arch/loader; make

#----------------------------------------------------------------------------------
dump:
	objdump -D -mi386 kernel.elf

image:
	tools/build arch/boot.bin arch/loader/loader.bin kernel.bin arch/b.img
	dd if=arch/b.img of=arch/c.img bs=512 count=128 conv=notrunc
tool:
	gcc -o tools/build tools/build.c
untool:
	rm tools/*.o tools/build
install:
	sudo dd if=arch/c.img of=/dev/sdb bs=512 count=128 conv=notrunc
cleanDebug:
	cd arch/; make cleanDebug
	rm init/*.o kernel/*.o lib/*.o mm/*.o *.bin *.elf *.dis
	rm Debug/ -rf
	cd arch/loader/; make cleanDebug;
#---------------------------------------------------------------------------------