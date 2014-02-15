# Makefile for MatrixOS

SOURCES=boot.o kernel.o monitor.o common.o descriptor_tables.o isr.o interrupt.o gdt.o timer.o \
        kheap.o paging.o ordered_array.o fs.o initrd.o task.o process.o syscall.o \

MODULES=$(INPUT) $(SYSTEM) $(FUN) $(GRAPHICS)

INPUT=input/i8042.o input/keyboard.o
SYSTEM=system/moduleLoading.o
FUN=fun/fun.o
GRAPHICS=graphics/graphics.o

CFLAGS=-nostdlib -nostdinc -fno-builtin -g
LDFLAGS=-Tlink.ld
ASFLAGS=-felf

CC=matrix-gcc
LD=matrix-ld

all: $(SOURCES) $(MODULES) link

clean:
	-rm *.o ../bin/MatrixOS.bin
	cd input; rm *.o; cd ..;

link:
	$(LD) $(LDFLAGS) -o ../bin/MatrixOS.bin $(SOURCES) $(MODULES)

.s.o:
	nasm $(ASFLAGS) $<
