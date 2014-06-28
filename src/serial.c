#include "serial.h"
#include "/home/grant/MatrixOS/include/stdarg.h"

#define DEFAULT_PORT	1

u8int current_port;


void init_serial ( u8int port, u16int divisor ) {
	u16int rate = 115200 / divisor;
	u32int io;

	switch ( port ) {
	case 1:
		io = PORT_COM1;
		break;

	case 2:
		io = PORT_COM2;
		break;

	case 3:
		io = PORT_COM3;
		break;

	case 4:
		io = PORT_COM4;
		break;

	default:
		io = DEFAULT_PORT;
		break;
	}

	outb ( io + 1, 0x00 ); // Disable all interrupts
	outb ( io + 3, 0x80 ); // Enable DLAB (set baud rate divisor)
	outb ( io + 0, ( rate & 0xFF ) ); //(lo byte) MAY BE IN WRONG ORDER
	outb ( io + 1, ( ( rate >> 8 ) & 0xFF ) ); //(hi byte)
	outb ( io + 3, 0x03 ); // 8 bits, no parity, one stop bit
	outb ( io + 2, 0xC7 ); // Enable FIFO, clear them, with 14-byte threshold
	outb ( io + 4, 0x0B ); // IRQs enabled, RTS/DSR set
	current_port = port;
}

int serial_received() {
	return inb ( PORT_COM1 + 5 ) & 1;
}

char read_serial() {
	while ( serial_received() == 0 );

	return inb ( PORT_COM1 );
}

int is_transmit_empty() {
	return inb ( PORT_COM1 + 5 ) & 0x20;
}


//TODO print message on old port about leaving.
void set_port ( u8int port ) {
	u32int io;

	switch ( port ) {
	case 1:
		io = PORT_COM1;
		break;

	case 2:
		io = PORT_COM2;
		break;

	case 3:
		io = PORT_COM3;
		break;

	case 4:
		io = PORT_COM4;
		break;

	default:
		io = DEFAULT_PORT;
		break;
	}

	current_port = io;
}

void write_serial ( char a ) {
	while ( is_transmit_empty() == 0 );

	u32int io;

	switch ( current_port ) {
	case 1:
		io = PORT_COM1;
		break;

	case 2:
		io = PORT_COM2;
		break;

	case 3:
		io = PORT_COM3;
		break;

	case 4:
		io = PORT_COM4;
		break;

	default:
		io = DEFAULT_PORT;
		break;
	}

	outb ( io, a );
}
void serialf ( char *c, ... ) {
	va_list arguments;

	int i, numberOfArgs = 0, stringLength = strlen ( c );
	int integerArg;

	for ( i = 0; c[i]; i++ ) //checks number arguments is correct
		if ( c[i] == '%' ) {
			numberOfArgs++;
		}

	i = 0;

	va_start ( arguments, *c );

	while ( c[i] ) {
		if ( c[i] == '%' ) {
			i++;

			if ( c[i] == 's' ) {
				char *charArrayArg;
				charArrayArg = va_arg ( arguments, char* );
				serialf ( charArrayArg );
			}

			if ( c[i] == 'd' ) {
				integerArg = va_arg ( arguments, int );

				int numberLength = math_intLength ( integerArg );
				int x;

				char integer[numberLength];

				intToChar ( integerArg, integer ); //sets integer as char

				numToAsciChar ( integer, numberLength ); //sets char to ASCII for printing

				for ( x = 0; x < numberLength; x++ ) {
					write_serial ( integer[x] );

				}
			}

			if ( c[i] == 'c' ) { //user wants to print a char
				char *charArg;
				charArg = va_arg ( arguments, char* );

				write_serial ( charArg[0] );

			}

			if ( c[i] == 'h' ) { //user wants to print a hex number

				u32int hexArg = va_arg ( arguments, u32int );

				s32int tmp;

				serialf ( "0x" );

				char noZeroes = 1;

				int hexCount;

				for ( hexCount = 28; hexCount > 0; hexCount -= 4 ) {
					tmp = ( hexArg >> hexCount ) & 0xF;

					if ( !tmp && noZeroes ) {
						continue;
					}

					if ( tmp >= 0xA ) {
						noZeroes = 0;
						write_serial ( tmp-0xA+'a' );

					} else {
						noZeroes = 0;
						write_serial ( tmp+'0' );
					}
				}

				tmp = hexArg & 0xF;

				if ( tmp >= 0xA ) {
					write_serial ( tmp - 0xA + 'a' );

				} else {
					write_serial ( tmp+'0' );
				}
			}

		} else {
			write_serial ( c[i] );
		}

		i++;
	}

	va_end ( arguments );
}


