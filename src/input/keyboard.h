#include "../isr.h"
#ifndef KEYBOARD_H
#define KEYBOARD_H


void keyboard_handler ( registers_t *regs );
s8int init_keyboard();
char getKey();


#endif
