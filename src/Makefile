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

CLEAN_CODE_DIRS_C=*.c input/*.c system/*.c fun/*.c graphics/*.c math/*.c
CLEAN_CODE_DIRS_H=*.h input/*.h system/*.h fun/*.h graphics/*.h math/*.h
CLEAN_CODE_DIRS_C_ORIG=*.c.orig input/*.c.orig system/*.c.orig fun/*.c.orig graphics/*.c.orig math/*.c.orig
CLEAN_CODE_DIRS_H_ORIG=*.h.orig input/*.h.orig system/*.h.orig fun/*.h.orig graphics/*.h.orig math/*.h.orig


all: $(SOURCES) $(MODULES) link

release: clean all

clean:
	-rm *.o ../bin/MatrixOS.bin
	cd input; rm *.o; cd ..;
	cd graphics; rm *.o; cd ..;

clean_code:
	echo "MatrixOS Cleaning code with astyle:"
	astyle --style=google --indent=force-tab --indent-cases --break-blocks=all --pad-paren --add-brackets $(CLEAN_CODE_DIRS_C) $(CLEAN_CODE_DIRS_H)
	cp 

link:
	$(LD) $(LDFLAGS) -o ../bin/MatrixOS.bin $(SOURCES) $(MODULES)

.s.o:
	nasm $(ASFLAGS) $<
