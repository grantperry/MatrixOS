#include "../isr.h"
#ifndef KEYBOARD_H
#define KEYBOARD_H


void keyboard_handler ( registers_t *regs );
s8int init_keyboard();
char getKey();

typedef void	( *keyboard_initialize ) ( );
typedef void	( *keyboard_lights ) (u8int Caps, u8int Scroll, u8int Num);

typedef struct keyboard {
	char driver_name[32];
	keyboard_initialize	*init;
	keyboard_lights		*lights;
} keyboard_t;


#endif
