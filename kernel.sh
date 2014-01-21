#!/bin/sh

MATRIX="/home/grant/MatrixOS/"
AS="nasm"

CARGS="-std=gnu99 -O2 -Wall -Wextra -fno-builtin -nostdlib -fno-stack-protector -Wno-unused-parameter -g"
#can add -nostdinc if there are conflicts(not suggested)
#remove -g to remove debug symbols

cd $MATRIX
cd src

echo "> Bootloader"
nasm -felf boot.asm -o ../build/boot.o

echo "> Kernel"
#matrix-gcc -c kernel.c -o ../build/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

echo ">> Kernel Header Files"

# TODO Make this automatic

matrix-gcc -c include/monitor.c -o ../build/monitor.o $CARGS

echo ">> Kernel Assembly Files"

#$AS -felf include/gdt/gdt.asm -o ../build/gdt.o
#$AS -felf include/gdt/interrupt.asm -o ../build/interrupt.o

cd $MATRIX
echo "> Link"
matrix-gcc -Tsrc/linker.ld -o bin/MatrixOS.bin -ffreestanding -O2 -nostdlib -lgcc

mkdir -p isodir
mkdir -p isodir/boot
cp bin/MatrixOS.bin isodir/boot/MatrixOS.bin
mkdir -p isodir/boot/grub
cp grub/grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o MatrixOS.iso isodir
