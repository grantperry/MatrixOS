SOURCES=initrd_gen.o

CFLAGS=
LDFLAGS=-T link.ld
ASFLAGS=-felf

CC=gcc
LD=ld

all: $(SOURCES) link

clean:
	-rm *.o initrd_gen

link:
	$(CC) -o initrd_gen $(SOURCES)
