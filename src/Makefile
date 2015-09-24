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

CLIB_PREFIX=~/opt/cross/bin

EX_DIR=~/opt/cross/bin
CC=$(CLIB_PREFIX)/i686-elf-gcc
LD=$(CLIB_PREFIX)/i686-elf-ld

export MATRIX_BASE CFLAGS LDFLAGS ASFLAGS .s.o CC LD

.PHONY: all

all: $(SOURCES) subdirs link

subdirs:
	@for d in $(SUBDIRS); do\
		$(MAKE) --no-print-directory -C $$d all; # get the first part of the path before the /\
	done;\


release: clean all

.PHONY: clean
clean:
	rm -f *.o $(MATRIX_BASE)/bin/MatrixOS.bin
	for c in $(SUBDIRS); do\
		$(MAKE) --no-print-directory -C $$c clean;\
	done;\


CODE := $(sort $(wildcard ./*.c))

clean_code: $(CLEAN_CODE_DIRS)
	@echo "[*] MatrixOS Cleaning code with astyle:" $(CODE)
	astyle --style=google --indent=force-tab --indent-cases --break-blocks=all --pad-paren --add-brackets $(CODE)
	rm -f *.orig

.PHONY: link
link:
	@echo "[*] Linking"
	$(LD) $(LDFLAGS) -o $(MATRIX_BASE)/bin/MatrixOS.bin boot.o $(filter-out ./boot.o, $(SOURCES)) $(SUBDIRSRC)

.PHONY: .s.o
.s.o:
	nasm $(ASFLAGS) $<
