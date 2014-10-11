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

LD_SCRIPT=link.ld

CFLAGS=-nostdlib -ffreestanding -nostdinc -g -I $(MATRIX_BASE)/include
#-fno-builtin
LDFLAGS=-T$(LD_SCRIPT)
ASFLAGS=-felf

EX_DIR=~/opt/cross/bin
CC=$(EX_DIR)/i686-elf-gcc
LD=$(EX_DIR)/i686-elf-ld

export MATRIX_BASE CFLAGS LDFLAGS ASFLAGS .s.o CC LD

.PHONY: all

all: $(SOURCES) subdirs link

subdirs: $(SUBDIRSRC)

$(SUBDIRSRC):
	@$(MAKE) --no-print-directory -C $(firstword $(subst /, ,$@)) all # get the first part of the path before the /


release: clean all

.PHONY: clean

clean:
	@rm -f *.o $(MATRIX_BASE)/bin/MatrixOS.bin
	@for c in $(DIRECTORY); do\
		$(MAKE) --no-print-directory -C $$c clean # get the first part of the path before the /\
	done;\

clean_code: $(CLEAN_CODE_DIRS)
	@echo "[*] MatrixOS Cleaning code with astyle:"
	astyle --style=google --indent=force-tab --indent-cases --break-blocks=all --pad-paren --add-brackets $(CLEAN_CODE_DIRS_C) $(CLEAN_CODE_DIRS_H)

link:
	@echo "[*] Linking"
	$(LD) $(LDFLAGS) -o $(MATRIX_BASE)/bin/MatrixOS.bin boot.o $(filter-out ./boot.o, $(SOURCES)) $(SUBDIRSRC)

.s.o:
	nasm $(ASFLAGS) $<
