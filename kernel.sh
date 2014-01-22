#!/bin/sh

MATRIX="/home/grant/MatrixOS/"

#for bochs...
BXSHARE=$MATRIX/../bios_bochs/

cd $MATRIX
cd src

make

cd $MATRIX


mkdir -p isodir/boot
cp bin/MatrixOS.bin isodir/boot/MatrixOS.bin
mkdir -p isodir/boot/grub
cp grub/grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o MatrixOS.iso isodir
