# MatrixOS
# Makefile for MatrixOS
MATRIX_BASE=~/MatrixOS

DIRECTORY := $(sort $(dir $(wildcard ./*/)))
DIRECTORY := $(filter-out ./,$(DIRECTORY))
SOURCES := $(sort $(wildcard ./*/))
SOURCES := $(filter-out ./Makefile,$(SOURCES))
SOURCES := $(filter-out ./Makefile~,$(SOURCES))
SOURCES := $(filter-out $(wildcard ./*~),$(SOURCES))
SOURCES := $(filter-out $(wildcard ./*.orig),$(SOURCES))
SOURCES := $(filter-out $(DIRECTORY),$(SOURCES))
SOURCES := $(SOURCES:.c=.o)
SOURCES := $(SOURCES:.s=.o)
SOURCES := $(filter-out $(wildcard ./*.h),$(SOURCES))
SOURCES := $(filter-out ./.h,$(SOURCES))
SOURCES := $(filter-out $(wildcard ./*.ld),$(SOURCES))
SOURCES := $(filter-out ./.ld,$(SOURCES))
SOURCES := $(sort $(SOURCES))


BUILD_DIR=$(MATRIX_BASE)/build

SUBDIRS := $(sort $(dir $(wildcard */)))
SUBDIRS := $(filter-out ./,$(SUBDIRS))

SUBDIRSRC = $(join $(SUBDIRS), $(SUBDIRS:/=_OUT.o))

INPUT=input/i8042.o input/keyboard.o
SYSTEM=system/moduleLoading.o system/shell.o
FUN=fun/fun.o
GRAPHICS=graphics/graphics.o graphics/x_server.o graphics/vesa.o graphics/gui.o
MATH=math/math.o
FS=fs/ext2.o
DRIVERS=drivers/sound.o

LD_SCRIPT=link.ld

CFLAGS=-nostdlib -ffreestanding -nostdinc -g
#-fno-builtin
LDFLAGS=-T$(LD_SCRIPT)
ASFLAGS=-felf

EX_DIR=~/opt/cross/bin
CC=$(EX_DIR)/i686-elf-gcc
LD=$(EX_DIR)/i686-elf-ld

CLEAN_CODE_DIRS_C=*.c input/*.c system/*.c fun/*.c graphics/*.c math/*.c
CLEAN_CODE_DIRS_H=*.h input/*.h system/*.h fun/*.h graphics/*.h math/*.h
CLEAN_CODE_DIRS=. input system fun graphics math

export MATRIX_BASE CFLAGS LDFLAGS ASFLAGS .s.o CC LD

all: $(SOURCES) subdirs link

subdirs: $(SUBDIRSRC)

$(SUBDIRSRC):
	@$(MAKE) --no-print-directory -C $(firstword $(subst /, ,$@)) # get the first part of the path before the /


release: clean all

clean: $(SUBDIRSRC)
	@rm -f *.o $(MATRIX_BASE)/bin/MatrixOS.bin
	@$(MAKE) --no-print-directory -C $(firstword $(subst /, ,$<)) clean # get the first part of the path before the /

clean_code: $(CLEAN_CODE_DIRS)
	@echo "[*] MatrixOS Cleaning code with astyle:"
	astyle --style=google --indent=force-tab --indent-cases --break-blocks=all --pad-paren --add-brackets $(CLEAN_CODE_DIRS_C) $(CLEAN_CODE_DIRS_H)

link:
	@echo "[*] Linking"
	$(LD) $(LDFLAGS) -o $(MATRIX_BASE)/bin/MatrixOS.bin boot.o $(filter-out ./boot.o, $(SOURCES)) $(SUBDIRSRC)

.s.o:
	nasm $(ASFLAGS) $<
