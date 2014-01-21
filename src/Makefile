SOURCES=boot.o kernel.o common.o monitor.o

CFLAGS=-nostdlib -nostdinc -fno-builtin -fno-stack-protector
LDFLAGS=-Tlinker.ld
ASFLAGS=-felf

all: $(SOURCES) link 

clean:
	-rm *.o kernel

link:
	ld $(LDFLAGS) -o ../bin/MatrixOS.bin $(SOURCES)

.s.o:
	nasm $(ASFLAGS) $<
