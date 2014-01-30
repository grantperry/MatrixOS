# Makefile for JamesM's kernel tutorials.
# The C and C++ rules are already setup by default.
# The only one that needs changing is the assembler 
# rule, as we use nasm instead of GNU as.

SOURCES=boot.o kernel.o monitor.o common.o descriptor_tables.o isr.o interrupt.o gdt.o timer.o \
        kheap.o paging.o ordered_array.o fs.o initrd.o task.o process.o syscall.o input/keyboard.o \
        

CFLAGS=-nostdlib -nostdinc -fno-builtin -g
LDFLAGS=-Tlink.ld
ASFLAGS=-felf

all: $(SOURCES) link

clean:
	-rm *.o ../bin/MatrixOS.bin

link:
	ld $(LDFLAGS) -o ../bin/MatrixOS.bin $(SOURCES)

.s.o:
	nasm $(ASFLAGS) $<
