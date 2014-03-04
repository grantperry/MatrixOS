#include "../isr.h"
#ifndef KEYBOARD_H
#define KEYBOARD_H


void keyboard_handler(registers_t *regs);
s8int init_keyboard();
void toggleCapsLock();

void keyboard_get(char* outStr);


#endif
