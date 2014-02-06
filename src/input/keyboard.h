#include "../isr.h"
#ifndef KEYBOARD_H
#define KEYBOARD_H


void keyboard_handler(registers_t *regs);
void init_keyboard();



void disable_devices();				//1
void flush_output_buffer();			//2
void set_controller_config_byte();	//3
void controller_self_test();		//4
void two_channels();				//5
void interface_test();				//6
void enable_devices();				//7
void reset_devices();				//8
void lable_devices();				//9

typedef struct keyboard_buffer_object {
	u8int command;
	u8int full;
} key_buf_object;



#endif
