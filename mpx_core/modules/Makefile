#
# Makefile for MPX Modules

AS      = nasm
CC      = i386-elf-gcc
CFLAGS  = -Wall -Wextra -Werror -nostartfiles -nostdlib -nodefaultlibs -ffreestanding -g -c
LD      = i386-elf-ld
LDFLAGS = 
ASFLAGS = -f elf -g

# add new files as you create them.
# I'll start you off with just mpx_supt.o
OBJFILES =\
mpx_supt.o\
polling_helper.o\
commandhandler.o\
pcb_internal.o\
pcb_commands.o\
procsr3.o\
alarm.o\
mem_management.o\
mpx_R6.o


.c.s:
	$(CC) $(CFLAGS) -S -o $@ $<
.s.o:
	$(AS) $(ASFLAGS) -o $@ $<
.c.o:
	$(CC) $(CFLAGS) -I../include -o $@ $<

all: modules.o

modules.o: $(OBJFILES)
	$(LD) -r -o modules.o $(OBJFILES)

clean:
	rm modules.o $(OBJFILES)
