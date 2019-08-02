
#-----------------------------------------------------------------------------------------------
# File Makefile for all the sources file to make the objects.
# 2016.04.18
#-----------------------------------------------------------------------------------------------
AS				= as
CC 				= gcc
LD 				= ld
OBJCOPY 		= objcopy
OBJDUMP			= objdump
ASFLAGS         = -W -g
CFLAGS 			= -Wall -O2 -g -I../include -fno-stack-protector -fno-builtin -nostdinc
LDFLAGS 		= -Tls.ld -Map ls.map
# --verbose     

ELF        = ls.elf
DIS        = ls.dis
BIN 		 = ls.bin

SRCS = $(wildcard *.S)
SRCC = $(wildcard *.c)

OBJS = $(patsubst %.S,%.o, $(SRCS))
OBJC = $(patsubst %.c,%.o, $(SRCC))

sh : $(OBJS) $(OBJC)
	$(LD) $(LDFLAGS) -o $(ELF) $(OBJS) $(OBJC) -L../lib/ -los
	$(OBJCOPY) -S -O binary  $(ELF) $(BIN)
	$(OBJDUMP) -D $(ELF) > $(DIS)
$(OBJS):$(SRCS)
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm *.bin *.elf *.o *.map *.dis
	
#---------------------------------------------------------------------------------
