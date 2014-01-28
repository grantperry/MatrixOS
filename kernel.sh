#!/bin/sh

MATRIX="/home/grant/MatrixOS/"

#for bochs...
BXSHARE=$MATRIX/../bios_bochs/

cd $MATRIX
cd src

make

cd $MATRIX




sudo /sbin/losetup /dev/loop0 MatrixOS.img
sudo mount /dev/loop0 /mnt2
sudo cp bin/MatrixOS.bin /mnt2/kernel
sudo umount /dev/loop0
sudo /sbin/losetup -d /dev/loop0

#mkdir -p isodir/boot
#cp bin/MatrixOS.bin isodir/boot/MatrixOS.bin
#mkdir -p isodir/boot/grub
#cp grub/grub.cfg isodir/boot/grub/grub.cfg
#grub-mkrescue -o MatrixOS.iso isodir
