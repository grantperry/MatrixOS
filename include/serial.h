// MatrixOS - serial.h
// Serial communication
#ifndef SERIAL_H
#define SERIAL_H

#include <common.h>

//Standard COM ports
#define PORT_COM1 0x3F8
#define PORT_COM2 0x2F8
#define PORT_COM3 0x3F8
#define PORT_COM4 0x2F8

//Standard IRQ Numbers
#define IRQ_COM1 4
#define IRQ_COM2 3
#define IRQ_COM3 4
#define IRQ_COM4 3

void init_serial ( u8int, u16int divisor );

int serial_received();

char read_serial();

int is_transmit_empty();

void write_serial ( char a );

void serialf ( char *c, ... );

#endif//SERIAL_H
