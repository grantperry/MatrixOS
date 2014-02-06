#include "../isr.h"
#ifndef KEYBOARD_H
#define KEYBOARD_H


void keyboard_handler(registers_t *regs);
void init_keyboard();

u8int keyboard_ctrl_read_status();
void keyboard_enc_send_cmd(u8int cmd);
void keyboard_set_leds(u8int num, u8int caps, u8int scroll);
u8int keyboard_self_test();
void keyboard_ctrl_send_cmd(u8int cmd);
u8int keyboard_enc_read_buf();



#endif
