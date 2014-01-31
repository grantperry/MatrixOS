#!/bin/sh

MATRIX="/home/grant/MatrixOS/"

#for bochs...
BXSHARE=$MATRIX/../bios_bochs/

cd $MATRIX
cd src

make

cd $MATRIX


mkdir -p isodir
mkdir -p isodir/boot
cp bin/MatrixOS.bin isodir/MatrixOS.bin
cp initrd.img       isodir/initrd
mkdir -p isodir/boot/grub
cp grub/grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o MatrixOS.iso isodir

#sudo mount -o loop MatrixOS.img /media/floppy1/
#sudo cp bin/MatrixOS.bin /media/floppy1/kernel
#sudo cp initrd.img       /media/floppy1/initrd
#sleep 0.1
#sudo umount /media/floppy1

#mkdir -p isodir/boot
#cp bin/MatrixOS.bin isodir/boot/MatrixOS.bin
#mkdir -p isodir/boot/grub
#cp grub/grub.cfg isodir/boot/grub/grub.cfg
#grub-mkrescue -o MatrixOS.iso isodir
