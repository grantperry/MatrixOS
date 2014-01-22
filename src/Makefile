SOURCES=boot.o kernel.o common.o monitor.o descriptor_tables.o gdt.o interupt.o isr.o register_interupts.o timer.o

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
