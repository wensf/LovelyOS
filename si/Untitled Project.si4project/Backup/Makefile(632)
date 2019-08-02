#-----------------------------------------------------------------------------------------------
# File Makefile for all the sources file to make the objects.
# 2016.04.18
#-----------------------------------------------------------------------------------------------
AS				= as
CC 				= gcc
LD 				= ld
OBJCOPY 		= objcopy
OBJDUMP			= objdump
CFLAGS 			= -Wall -g -O1 -Iinclude -I include/lib -I include/kernel -I include/mm -I include/init -I include/fs -I include/driver -fno-stack-protector -fno-builtin -nostdinc
LDFLAGS 		= -T.lds
BOOT_BIN        = boot.bin
LOADER_BIN      = loader.bin
START_BIN		= kernel/start.bin
KERNEL_BIN 		= kernel.bin
IMAGE			= arch/a.img
IMAGE_MOUNT_AT	= /mnt/floppy

SRCS_KERNEL = kernel/system_call.S kernel/kernel.S
SRCC_KERNEL = $(wildcard kernel/*.c)
SRCC_INIT   = $(wildcard init/*.c)
SRCC_LIB    = $(wildcard lib/*.c)
SRCS_LIB    = $(wildcard lib/*.S)
SRCC_MM     = $(wildcard mm/*.c)
SRCC_FS     = $(wildcard fs/*.c)
SRCC_DRIVER = $(wildcard driver/*.c)

OBJS_KERNEL = $(patsubst %.S,%.o, $(SRCS_KERNEL))
OBJC_KERNEL = $(patsubst %.c,%.o, $(SRCC_KERNEL))
OBJC_INIT   = $(patsubst %.c,%.o, $(SRCC_INIT))
OBJC_LIB    = $(patsubst %.c,%.o, $(SRCC_LIB))
OBJS_LIB    = $(patsubst %.S,%.o, $(SRCS_LIB))
OBJC_MM     = $(patsubst %.c,%.o, $(SRCC_MM))
OBJC_FS     = $(patsubst %.c,%.o, $(SRCC_FS))
OBJC_DRIVER = $(patsubst %.c,%.o, $(SRCC_DRIVER))

Debug : $(BOOT_BIN) $(LOADER_BIN) $(START_BIN) $(OBJS_KERNEL) $(OBJC_KERNEL) $(OBJS_LIB) $(OBJC_LIB) $(OBJC_MM) $(OBJC_INIT) $(OBJC_FS)  $(OBJC_DRIVER)
	$(LD) $(LDFLAGS) --oformat elf32-i386 -o kernel.elf $(OBJS_KERNEL) $(OBJC_KERNEL) $(OBJS_LIB) $(OBJC_LIB)  $(OBJC_MM) $(OBJC_INIT) $(OBJC_FS) $(OBJC_DRIVER)
	$(OBJCOPY) -S -O binary kernel.elf $(KERNEL_BIN)
	$(OBJDUMP) -D kernel.elf > kernel.dis
	make shell
	make image

$(BOOT_BIN):
	cd arch/; make

$(LOADER_BIN):
	cd arch/loader; make

kernel/start.o:kernel/start.S

kernel/start.bin:kernel/start.o
	$(LD) -Ttext=0x106000 --oformat elf32-i386  kernel/start.o -o kernel/start.elf
	$(OBJCOPY) -S --only-section .text --only-section .rodata --only-section .data -O binary kernel/start.elf $(START_BIN)
	$(OBJDUMP) -D kernel/start.elf > kernel/start.dis

#----------------------------------------------------------------------------------
run:
	bochs -f bochsrc.bxrc
dump:
	objdump -D -mi386 kernel.elf
image:
	tools/build arch/b.img arch/boot.bin arch/loader/loader.bin kernel/start.bin kernel.bin user/sh.bin tools/logo.bin
	dd if=arch/b.img of=arch/c.img bs=512 count=512 conv=notrunc
tool:
	gcc -o tools/build tools/build.c
	
shell:
	cd user; make
untool:
	rm tools/*.o tools/build
install:
	sudo dd if=arch/c.img of=/dev/sdb bs=512 count=512 conv=notrunc
clean:
	cd arch/; make cleanDebug
	rm init/*.o kernel/*.o lib/*.o mm/*.o fs/*.o driver/*.o kernel/*.bin kernel/*.elf kernel/*.dis *.bin *.elf *.dis
	rm Debug/ -rf
	cd arch/loader/; make cleanDebug;
	cd user/; make clean;
#---------------------------------------------------------------------------------









