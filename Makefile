DIR = src
SOURCES = $(shell find $(DIR) -name "*.c")
OBJECTS := $(SOURCES:%.c=obj/%.o)

all:
	print $(SOURCES) $(OBJECTS)
