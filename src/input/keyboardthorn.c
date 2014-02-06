#include "keyboard.h"
#include "../isr.h"
#include "../common.h"

u8int keyboard_1_Disabled = 0;
u8int KEYBOARD_INITALIZED = 0;


//! read status from keyboard controller
u8int keyboard_ctrl_read_status () {
	return inb (0x64);
}

//! send command byte to keyboard controller
void keyboard_ctrl_send_cmd (u8int cmd) {
 
	//! wait for kkybrd controller input buffer to be clear
	while (1) {
		if ( (keyboard_ctrl_read_status() & 0x02) == 0) {
			break;
		}
	}
 
	outb (0x64, cmd);
}

void keyboard_set_leds (u8int num, u8int caps, u8int scroll) {
 
	u8int data = 0;
 
	//! set or clear the bit
	data = (scroll) ? (data | 1) : (data & 1);
	data = (num) ? (num | 2) : (num & 2);
	data = (caps) ? (num | 4) : (num & 4);
 
	//! send the command -- update keyboard Light Emetting Diods (LEDs)
	keyboard_enc_send_cmd (0xED);
	keyboard_enc_send_cmd (data);
}

//! read keyboard encoder buffer
u8int keyboard_enc_read_buf () {
 
	return inb(0x60);
}
 
//! send command byte to keyboard encoder
void keyboard_enc_send_cmd (u8int cmd) {
 
	//! wait for kkybrd controller input buffer to be clear
	while (1)
		if ( (keyboard_ctrl_read_status () & 0x02) == 0)
			break;
 
	//! send command byte to kybrd encoder
	outb (0x60, cmd);
}

//! run self test
u8int keyboard_self_test () {
 
	//! send command
	keyboard_ctrl_send_cmd (0xAA); //Self Test
 
	//! wait for output buffer to be full
	while (1) {
		if (keyboard_ctrl_read_status () & 0x01) {
			break;
		}
	}
 
	//! if output buffer == 0x55, test passed
	return (keyboard_enc_read_buf() == 0x55) ? 0x1 : 0x0;
}

//! disables the keyboard
void keyboard_disable() {
 
	keyboard_ctrl_send_cmd (0xAD);
	keyboard_1_Disabled = 1;
}

//! enables the keyboard
void kkybrd_enable() {
	keyboard_ctrl_send_cmd (0xAE);
	keyboard_1_Disabled = 0;
}

void init_keyboard() { /////////////////////////////////////////////////////////
	KEYBOARD_INITALIZED = 0;
	register_interrupt_handler(IRQ1, &keyboard_handler);
	keyboard_set_leds(0,1,0);
	keyboard_self_test();
	
	keyboard_disable();
	
	outb(0x64, 0x20); //ask for config byte
	u8int config = inb(0x60);
	config = 0b00000111;
	outb(0x64, 0x60);
	outb(0x60, config);
	outb(0x64, 0xAE);
	outb(0x64, 0xA8);
	asm volatile("sti");
	
	
	KEYBOARD_INITALIZED = 1;
	keyboard_set_leds(0,1,0);
}

void keyboard_handler(registers_t *regs) {
	syscall_monitor_write("pre init press\n");
	if (KEYBOARD_INITALIZED) {
		syscall_monitor_write("key pressed:");
		u8int i = inb(0x60);
	}
}
