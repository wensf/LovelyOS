
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
CFLAGS 			= -Wall -O2 -g -I ./ -fno-stack-protector -fno-builtin -nostdinc
LDFLAGS 		= -Tsh.lds -Map sh.map
# --verbose

SHELL_ELF        = sh.elf
SHELL_DIS        = sh.dis
SHELL_BIN 		 = sh.bin

SRCS_SHELL = $(wildcard *.S)
SRCC_SHELL = $(wildcard *.c)

OBJS_SHELL = $(patsubst %.S,%.o, $(SRCS_SHELL))
OBJC_SHELL = $(patsubst %.c,%.o, $(SRCC_SHELL))

sh : $(OBJS_SHELL) $(OBJC_SHELL)
	$(LD) $(LDFLAGS) -o $(SHELL_ELF) $(OBJS_SHELL) $(OBJC_SHELL)
	$(OBJCOPY) -S -O binary  $(SHELL_ELF) $(SHELL_BIN)
	$(OBJDUMP) -D $(SHELL_ELF) > $(SHELL_DIS)
$(OBJS_SHELL):$(SRCS_SHELL)
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm *.bin *.elf *.o *.map *.dis
	
#---------------------------------------------------------------------------------
