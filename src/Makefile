# Makefile for MatrixOS

SOURCES=boot.o kernel.o monitor.o common.o descriptor_tables.o isr.o interrupt.o gdt.o timer.o \
        kheap.o paging.o ordered_array.o fs.o initrd.o task.o process.o syscall.o \
        v86.o stdio.o

MODULES=$(INPUT) $(SYSTEM) $(FUN) $(GRAPHICS) $(MATH)

INPUT=input/i8042.o input/keyboard.o input/special_keys.o
SYSTEM=system/moduleLoading.o system/shell.o
FUN=fun/fun.o
GRAPHICS=graphics/graphics.o graphics/x_server.o graphics/vesa.o
MATH=math/math.o

CFLAGS=-nostdlib -ffreestanding -nostdinc -g
#-fno-builtin
LDFLAGS=-Tlink.ld
ASFLAGS=-felf

CC=matrix-gcc
LD=matrix-ld

all: $(SOURCES) $(MODULES) link

release: clean all

clean:
	-rm *.o ../bin/MatrixOS.bin
	cd input; rm *.o; cd ..;
	cd graphics; rm *.o; cd ..;

link:
	$(LD) $(LDFLAGS) -o ../bin/MatrixOS.bin $(SOURCES) $(MODULES)

.s.o:
	nasm $(ASFLAGS) $<
