// MatrixOS - cpu.h
// Functions related to cpu
#ifndef CPU_H
#define CPU_H

#include "common.h"
#include "stdio.h"

//Read CMOS clock
unsigned char readCMOS ( unsigned char addr );

//Write CMOS clock
void writeCMOS ( unsigned char addr, unsigned int value );

void cpu_reboot();

void cpu_sti();

void cpu_cli();

void cpu_halt();

#endif //CPU_H
