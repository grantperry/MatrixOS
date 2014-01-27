#!/bin/sh

MATRIX="/home/grant/MatrixOS/"

#for bochs...
BXSHARE=$MATRIX/../bios_bochs/

cd $MATRIX
cd src

make

cd $MATRIX


sudo /sbin/losetup /dev/loop0 floppy.img
sudo mount /dev/loop0 /mnt2
sudo cp bin/MatrixOS.bin /mnt2/kernel
sudo umount /dev/loop0
sudo /sbin/losetup -d /dev/loop0
