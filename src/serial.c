#include "serial.h"

void init_serial(u16int divisor) {
	u16int rate = 115200 / divisor;
	printf("[COM1] setting rate: %d %d %d\n", rate, (rate & 0xFF), ((rate >> 8) & 0xFF));
	outb(PORT_COM1 + 1, 0x00);    // Disable all interrupts
	outb(PORT_COM1 + 3, 0x80);    // Enable DLAB (set baud rate divisor)
	outb(PORT_COM1 + 0, (rate & 0xFF)); //(lo byte) MAY BE IN WRONG ORDER
	outb(PORT_COM1 + 1, ((rate >> 8) & 0xFF));    //(hi byte)
	outb(PORT_COM1 + 3, 0x03);    // 8 bits, no parity, one stop bit
	outb(PORT_COM1 + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
	outb(PORT_COM1 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

int serial_received() {
   return inb(PORT_COM1 + 5) & 1;
}
 
char read_serial() {
   while (serial_received() == 0);
 
   return inb(PORT_COM1);
}

int is_transmit_empty() {
   return inb(PORT_COM1 + 5) & 0x20;
}
 
void write_serial(char a) {
   while (is_transmit_empty() == 0);
 
   outb(PORT_COM1, a);
}
