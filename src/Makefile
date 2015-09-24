# MatrixOS src file make script.

# Base Dir of Matrix for file location purposes: automatically at user home /MatrixOS
MATRIX_DIR_HOME=~/MatrixOS

# src directory for uncompiled code
MATRIX_DIR_SRC=$(MATRIX_DIR_HOME)/src

# bin directory for compiled binaries ready for use
MATRIX_DIR_BIN=$(MATRIX_DIR_HOME)/bin

# inited location. contains creator and initrd
MATRIX_DIR_INITRD=$(MATRIX_DIR_HOME)/initrd

# include directory. holds header files for MatrixOS
MATRIX_DIR_INCLUDE=$(MATRIX_DIR_HOME)/include


.PHONY: all clean test

# Just to not leave anything to chance
WARNINGS := -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
	-Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
	-Wredundant-decls -Wnested-externs -Winline -Wno-long-long \
	-Wuninitialized -Wconversion -Wstrict-prototypes

# Flags for GCC.
CFLAGS=-nostdlib -ffreestanding -nostdinc -g -I $(MATRIX_DIR_INCLUDE) $(WARNINGS)

# Flags for the linker.
LDFLAGS=-T$(LD_SCRIPT)

# Flags for NASM
ASFLAGS=-felf


SRC_FILES := $(shell find . -type f -name "\*.c")

all: 


$(MATRIX_DIR_BUILD)//%.o: $(shell pwd)/%.c
    $(CC) $(CFLAGS) $(LDFLAGS) $(SOURCETREE) -c $< -o $@
