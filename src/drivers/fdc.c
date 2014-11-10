#include <drivers/fdc.h>

void init_dma() {
	virtual_map_pages ( 0x00F000, 64000, 1, 0 );
	outb (0x0a, 0x06)      ; //mask DMA channel 2 and 0 (assuming 0 is already masked)
    outb (0x0c, 0xFF)      ; //reset the master flip-flop
    outb (0x04, 0x00)      ; //address to 0 (low byte)
    outb (0x04, 0xF0)      ; //address to 0x10 (high byte)
    outb (0x0c, 0xFF)      ; //reset the master flip-flop (again!!!)
    outb (0x05, 0xFF)      ; //count to 0x23ff (low byte)
    outb (0x05, 0x23)      ; //count to 0x23ff (high byte),
    outb (0x81, 0x00)      ; //external page register to 0 for total address of 00 10 00
    outb (0x0a, 0x02)      ; //unmask DMA channel 2
    return;
}

void prepare_for_floppy_DMA_write() {
    outb (0x0a, 0x06)      ; //mask DMA channel 2 and 0 (assuming 0 is already masked)
    outb (0x0b, 0x5A)      ; //01011010
    //single transfer, address increment, autoinit, write, channel2)
    outb (0x0a, 0x02)      ; //unmask DMA channel 2
    return;
}

void prepare_for_floppy_DMA_read(){
    outb (0x0a, 0x06)      ; //mask DMA channel 2 and 0 (assuming 0 is already masked)
    outb (0x0b, 0x56)      ; //01010110
    //single transfer, address increment, autoinit, read, channel2)
    outb (0x0a, 0x02)      ; //unmask DMA channel 2
    return;
}

void init_fdc() {
	serialf("Init fdc\n");
	init_dma();
	prepare_for_floppy_DMA_read();
	
	
}
