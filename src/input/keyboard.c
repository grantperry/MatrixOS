#include "keyboard.h"
#include "../isr.h"
#include "../common.h"

void keyboard_handler(registers_t *regs);

u8int read_status_register(u8int x);
u8int send_command(u16int dev, u8int com);
u8int PS2_1 = 0;
u8int PS2_2 = 0;

void init_keyboard() {
	register_interrupt_handler(IRQ1, &keyboard_handler);
	print("keyboard init\n");
	
	u8int null = inb(0x60);
	
	outb(0x64, 0x20);
	u8int stat = inb(0x60);
	//stat = 0b00000001;
	outb(0x64, 0x60);
	u8int s = inb(0x64);
	if (!(s & (1 << 1))) {
		outb(0x60, stat);
	}
	outb(0x64, 0xAA);
	stat = inb(0x60);
	if (stat == 0x55) {} else {print("FAILED:(\n");}
	
	outb(0x64, 0xA8); //Enable Second PS/2 port
	
	outb(0x64, 0x20); //Read confiuration Bit
	stat = inb(0x60);
	if (!(stat & (1 << 5))) {print("BIT 5 Clear\n"); outb(0x64, 0xA7);}
	stat &= ~(1 << 0);
	stat &= ~(1 << 1);
	stat &= ~(1 << 6);
	outb(0x64, 0x60);
	outb(0x60, stat);
	
	outb(0x64, 0xAB); //Test PS/2 No.1
	stat = inb(0x60);
	if (stat == 0x00) {
		print("PS/2 1 [WORKS]\n");
		PS2_1 = 1;
	}
	outb(0x64, 0xA9); //Test PS/2 No.2
	stat = inb(0x60);
	if (stat == 0x00) {
		print("PS/2 2 [WORKS]\n");
		PS2_2 = 1;
	}
	if (PS2_1) {
	print("hello1\n");
		outb(0x64, 0xAE); //enable PS/2 1
	}
	if(PS2_2) {
	print("hello2\n");
		outb(0x64, 0xA8); //enable PS/2 2
	}
	
	outb(0x64, 0x20); //Read confiuration Bit
	stat = inb(0x60);
	stat |= 1 << 0;
	stat |= 1 << 1;
	outb(0x64, 0x60);
	outb(0x60, stat);
	
	while(0 == read_status_register(1) || PS2_1 == 1) { //PS/2 1
		print("reset1\n");
		outb(0x60, 0xFF);
		u8int in = 0;
		while(1 == read_status_register(0)) { //check if all ok
			in = inb(0x60);
			if (in == 0xFA) {
				print("1 works\n");
			} else {
				print("1 failed\n");
				PS2_1 = 0;
			}
		}
	break;
	}
	
	while(0 == read_status_register(1) || PS2_2 == 1) { //PS/2 2
		print("reset2\n");
		outb(0x64, 0xD4);
		outb(0x60, 0xFF);
		u8int in = 0;
		while(1 == read_status_register(0)) { //check if all ok
			in = inb(0x60);
			if (in == 0xFA) {
				print("2 works\n");
			} else {
				print("2 failed\n");
				PS2_1 = 0;
			}
		}
		break;
	}
}

u8int read_status_register(u8int x) {
	u8int stat = inb(0x64);
	u8int ret;
	ret = ((stat >> x) & 1);
	monitor_write_dec(ret);
	return ret;
}

void keyboard_handler(registers_t *regs) {
	print("key pressed:)\n");
}
