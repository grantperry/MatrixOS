SOURCES=initrd_gen.o

CFLAGS=
LDFLAGS=-T link.ld
ASFLAGS=-felf

CC=gcc
LD=ld

all: $(SOURCES) link genrd

clean:
	-rm *.o initrd_gen

link:
	$(CC) -o initrd_gen $(SOURCES)

genrd:
	./initrd_gen falt.bin flat.bin flat2.bin flat2.bin
