#!/usr/bin/make
all:
	@make -f Makefile-server

QEMU=~/homebrew/bin/qemu-system-i386
M_HOME=~/MatrixOS
run: all
	@. ~/.profile; $(QEMU) -serial stdio -m 1024 -cdrom  $(M_HOME)/MatrixOS.iso

clean:
	@ $(MAKE) -C $(M_HOME)/src/ clean
