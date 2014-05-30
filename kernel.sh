#!/bin/sh

MATRIX="/home/grant/MatrixOS/"

cd $MATRIX
cd src;
make

cd $MATRIX


mkdir -p isodir
mkdir -p isodir/boot
cp bin/MatrixOS.bin isodir/MatrixOS.bin
cp initrd/initrd.img       isodir/initrd
mkdir -p isodir/boot/grub
cp grub/grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o MatrixOS.iso isodir
