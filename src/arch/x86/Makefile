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

SOURCES1 := $(subst ./, ,$(SOURCES))
SOURCES = $(SOURCES1)

OBJECTS := $(SOURCES:.c=.o)
OBJECTS := $(filter-out $(notdir $(shell pwd))_OUT.o,$(OBJECTS))
OBJECTS := $(filter-out Makefile,$(OBJECTS))

SUBDIRS := $(sort $(dir $(wildcard */)))
SUBDIRS := $(filter-out ./,$(SUBDIRS))

SUBDIRSRC = $(join $(SUBDIRS), $(SUBDIRS:/=_OUT.o))

export MATRIX_BASE CFLAGS LDFLAGS ASFLAGS .s.o CC LD

OUT = $(shell pwd)/$(notdir $(shell pwd))_OUT.o

.PHONY: all

all: subdirs $(OBJECTS) $(DIRECTORY) $(OUT) link

$(OUT): $(OBJECTS) $(DIRECTORY)

link: 
	@echo "[*] Linking" $(shell pwd)
	@rm -f $(shell pwd)/$(notdir $(shell pwd))_OUT.o
	@$(LD) -r -o $(shell pwd)/$(notdir $(shell pwd))_OUT.o $(OBJECTS) $(SUBDIRSRC)

.PHONY: clean

clean:
	@echo "cleaning directory:"  $(shell pwd)
	@rm -f *.o
	@for c in $(firstword $(subst /, ,$(SUBDIRSRC:_OUT.o=))); do\
		$(MAKE) --no-print-directory -C $$c clean # get the first part of the path before the /\
	done;\

CODE := $(sort $(wildcard ./*))
CODE := $(filter-out ./Makefile,$(CODE))
CODE := $(filter-out ./Makefile~,$(CODE))
CODE := $(filter-out $(DIRECTORY),$(CODE))
CODE := $(filter-out ./.h,$(CODE))

clean_code:
	@echo "[*] MatrixOS Cleaning code with astyle: " $(shell pwd)
	@astyle --style=google --indent=force-tab --indent-cases --break-blocks=all --pad-paren --add-brackets 

subdirs:
	@for d in $(SUBDIRS); do\
		$(MAKE) --no-print-directory -C $$d all # get the first part of the path before the /\
	done;\

.s.o:
	nasm $(ASFLAGS) $<
