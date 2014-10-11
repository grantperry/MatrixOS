DIRECTORY := $(sort $(dir $(wildcard ./*/)))
DIRECTORY := $(filter-out ./,$(DIRECTORY))
SOURCES := $(sort $(wildcard ./*/))
SOURCES := $(filter-out ./Makefile,$(SOURCES))
SOURCES := $(filter-out ./Makefile~,$(SOURCES))
SOURCES := $(filter-out $(wildcard ./*~),$(SOURCES))
SOURCES := $(filter-out $(wildcard ./*.orig),$(SOURCES))
SOURCES := $(filter-out $(DIRECTORY),$(SOURCES))
SOURCES := $(filter-out $(wildcard ./*.h),$(SOURCES))
SOURCES := $(filter-out ./.h,$(SOURCES))

SOURCES1 := $(subst ./, ,$(SOURCES))
SOURCES = $(SOURCES1)

OBJECTS := $(sort $(SOURCES:.c=.o))
OBJECTS := $(filter-out $(notdir $(shell pwd))_OUT.o,$(OBJECTS))
OBJECTS := $(filter-out Makefile,$(OBJECTS))

SUBDIRS := $(sort $(dir $(wildcard */)))
SUBDIRS := $(filter-out ./,$(SUBDIRS))

SUBDIRSRC = $(join $(SUBDIRS), $(SUBDIRS:/=_OUT.o))


all: subdirs $(OBJECTS) $(DIRECTORY) link

OUT := $(sort $(notdir $(wildcard ./*.o)))
OUT := $(filter-out ./$(notdir $(shell pwd))_OUT.o,$(OUT))
OUT := $(filter-out ./.o,$(OUT))

link: 
	@echo "[*] Linking" $(shell pwd)
	@rm -f $(shell pwd)/$(notdir $(shell pwd))_OUT.o
	@$(LD) -r -o $(shell pwd)/$(notdir $(shell pwd))_OUT.o $(OBJECTS)
	@echo "[*]"

clean: $(SUBDIRSRC)
	@echo "clean directory: " $<
	@rm -f *.o 
	$(ifeq "./" "./$(firstword $(subst /, ,$<))"\then\$(MAKE) --no-print-directory -C $(firstword $(subst /, ,$<)) clean\fi;)
	

CODE := $(sort $(wildcard ./*))
CODE := $(filter-out ./Makefile,$(CODE))
CODE := $(filter-out ./Makefile~,$(CODE))
CODE := $(filter-out $(DIRECTORY),$(CODE))
CODE := $(filter-out ./.h,$(CODE))

clean_code:
	@echo "[*] MatrixOS Cleaning code with astyle: " $(shell pwd)
	@astyle --style=google --indent=force-tab --indent-cases --break-blocks=all --pad-paren --add-brackets 

subdirs: $(SUBDIRSRC)

$(SUBDIRSRC):
	@$(MAKE) --no-print-directory -C $(firstword $(subst /, ,$@)) # get the first part of the path before the /
