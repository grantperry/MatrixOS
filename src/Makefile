# Makefile for MatrixOS
MATRIX_BASE=~/MatrixOS

SOURCES=boot.o kernel.o monitor.o common.o descriptor_tables.o isr.o interrupt.o gdt.o timer.o \
        kheap.o paging.o ordered_array.o fs.o initrd.o task.o process.o syscall.o \
        v86.o stdio.o elf_loader.o cpu.o vfs.o serial.o

MODULES=$(INPUT) $(SYSTEM) $(FUN) $(GRAPHICS) $(MATH) $(FS) $(DRIVERS)
SUBDIRS=fs

INPUT=input/i8042.o input/keyboard.o input/special_keys.o
SYSTEM=system/moduleLoading.o system/shell.o
FUN=fun/fun.o
GRAPHICS=graphics/graphics.o graphics/x_server.o graphics/vesa.o graphics/gui.o
MATH=math/math.o
FS=fs/ext2.o
DRIVERS=drivers/pci.o drivers/pcie.o

LD_SCRIPT=link.ld

CFLAGS=-nostdlib -ffreestanding -nostdinc -g
#-fno-builtin
LDFLAGS=-T$(LD_SCRIPT)
ASFLAGS=-felf

EX_DIR=~/opt/cross/bin
CC=$(EX_DIR)/i686-elf-gcc
LD=$(EX_DIR)/i686-elf-ld

CLEAN_CODE_DIRS_C=*.c input/*.c system/*.c fun/*.c graphics/*.c math/*.c fs/*c drivers/*c
CLEAN_CODE_DIRS_H=*.h input/*.h system/*.h fun/*.h graphics/*.h math/*.h fs/*h drivers/*h
CLEAN_CODE_DIRS=. input system fun graphics math


all: $(SOURCES) $(MODULES) link

release: clean all

clean:
	-rm *.o $(MATRIX_BASE)/bin/MatrixOS.bin
	cd input; rm *.o; cd ..;
	cd graphics; rm *.o; cd ..;
	cd fun; rm *.o; cd ..;
	cd math; rm *.o; cd ..;
	cd system; rm *.o; cd ..;
	cd fs; rm *.o; cd ..;
	cd drivers; rm *.o; cd ..;

clean_code: $(CLEAN_CODE_DIRS)
	@echo "[*] MatrixOS Cleaning code with astyle:"
	astyle --style=google --indent=force-tab --indent-cases --break-blocks=all --pad-paren --add-brackets $(CLEAN_CODE_DIRS_C) $(CLEAN_CODE_DIRS_H)

link:
	@echo "[*] Linking"
	@$(LD) $(LDFLAGS) -o $(MATRIX_BASE)/bin/MatrixOS.bin $(SOURCES) $(MODULES)
.s.o:
	nasm $(ASFLAGS) $<
