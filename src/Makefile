SOURCES=boot.o kernel.o monitor.o common.o descriptor_tables.o isr.o interrupt.o gdt.o timer.o \
        kheap.o paging.o register_interupts.o ordered_map.o

CFLAGS=-nostdlib -nostdinc -fno-builtin -fno-stack-protector
LDFLAGS=-Tlink.ld
ASFLAGS=-felf

all: $(SOURCES) link

clean:
	-rm *.o

link:
	ld $(LDFLAGS) -o ../bin/MatrixOS.bin $(SOURCES)

.s.o:
	nasm $(ASFLAGS) $<
