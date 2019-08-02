
#-----------------------------------------------------------------------------------------------
# File Makefile for all the sources file to make the objects.
# 2016.04.18
#-----------------------------------------------------------------------------------------------
AS				= as
CC 				= gcc
CFLAGS 			= -Wall -O2 -g -I ../include -fno-stack-protector -fno-builtin -nostdinc
# --verbose

LIB_NAME  = libos.a

SRCS_LIBC = $(wildcard *.S)
SRCC_LIBC = $(wildcard *.c)

OBJS_LIBC = $(patsubst %.S,%.o, $(SRCS_LIBC))
OBJC_LIBC = $(patsubst %.c,%.o, $(SRCC_LIBC))

libc : $(OBJS_LIBC) $(OBJC_LIBC)
	ar crv $(LIB_NAME) $(OBJS_LIBC) $(OBJC_LIBC)
$(OBJS_LIBC):$(SRCS_LIBC)
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm *.o $(LIB_NAME) 
	
#---------------------------------------------------------------------------------
