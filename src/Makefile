# Makefile for MatrixOS

SOURCES=boot.o kernel.o monitor.o common.o descriptor_tables.o isr.o interrupt.o gdt.o timer.o \
        kheap.o paging.o ordered_array.o fs.o initrd.o task.o process.o syscall.o \

MODULES=$(INPUT)

INPUT=input/i8042.o input/keyboard.o 

CFLAGS=-nostdlib -nostdinc -fno-builtin -g
LDFLAGS=-Tlink.ld
ASFLAGS=-felf

all: $(SOURCES) $(MODULES) link

clean:
	-rm *.o ../bin/MatrixOS.bin

link:
	ld $(LDFLAGS) -o ../bin/MatrixOS.bin $(SOURCES) $(MODULES)

.s.o:
	nasm $(ASFLAGS) $<
