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
	./initrd_gen 1 1 2 2 3 3
