#include "keyboard.h"
#include "../isr.h"
#include "../common.h"

u8int BOOL_KEY_PRESSED = 0;

u8int status = 0;
u8int config = 0;
u8int PS2_1 = 0;
u8int PS2_2 = 0;

key_buf_object COMMAND_BUFFER[8];
void init_buffer() {
	u8int i = 0;
	for(i == 7; i++;) {
		COMMAND_BUFFER[i].full = 0;
		COMMAND_BUFFER[i].command = 0;
	}
}

void has_key_pressed();

void init_keyboard() { /////////////////////////////////////////////////////////
	register_interrupt_handler(IRQ1, &keyboard_handler);
	
	init_buffer();
	
	has_key_pressed();
	disable_devices();
	flush_output_buffer();
	set_controller_config_byte();
	controller_self_test();
	two_channels();
	interface_test();
	enable_devices();
	reset_devices();
	lable_devices();
}

void has_key_pressed() {
	u8int t = 1;
	while (t) {
		u8int sta = inb(0x64);
		u8int bit = sta & (1 << 0);
		t++;
		if (bit == 1 || t == 250) {
			t = 0;
		}
	}
	return;
}

u8int in_stat();

void out_cont(u8int out) {
	u16int y = 0;
	outb(0x64, out);
	return;
}

u8int in_config() {
	out_cont(0x20);
	return inb(0x60);
}

void out_config(u8int conf) {
	out_cont(0x60);
	outb(0x60, conf);
}

u8int in_stat() {
	return inb(0x64);
}

u8int in() { //read from port 0x60
	status = in_stat();
	u16int d = 0;
	while((status & (1 << 1))) {
		d++;
		if(d == 60000) {return 0;}
	}
	return inb(0x60);
	
}

void out_key(u8int code) {
	u8int stat;
	while(1) {
		if((in_stat() & 0x02) == 0) {
			outb(0x60, code);
			return;
		}
		
		
	}
}

void disable_devices() {	//1
	syscall_monitor_write("\n1. disabling devices...\n");
	out_cont(0xAD);
	out_cont(0xA7);
}

void flush_output_buffer() {	//2
	syscall_monitor_write("2. flushing output buffer\n");
	status = in_stat();
	while((!(status >> 1) & 0x01)){u8int i = inb(0x60);}
}

void set_controller_config_byte() {	//3
	syscall_monitor_write("3. setting controller config byte\n");
	status = in_config();
	if (((status >> 5) & 0x01)) {
		PS2_2 = 0;
		syscall_monitor_write("   PS/2 2 = NO\n");
	} else {
		PS2_2 = 1;
		syscall_monitor_write("   PS/2 2 = YES\n");
	}
	status &= ~(1 << 0);
	status &= ~(1 << 1);
	status &= ~(1 << 6);
	out_config(status);
}

void controller_self_test() {	//4
	syscall_monitor_write("4. controller self test\n");
	out_cont(0xAA);
	u8int resp = 0;
	has_key_pressed();
	resp = in();
	resp = in();
	resp = in();
	if (resp == 0x55) {syscall_monitor_write("   Pass\n");}
	if (resp == 0xFC) {syscall_monitor_write("   FAIL!!!\n");}
}

void two_channels() {	//5
	syscall_monitor_write("5. finding channels\n");
	out_cont(0xA8);
	has_key_pressed();
	config = in_config();
	PS2_2 = !((status >> 5) & 0x01);
	if(PS2_2) {
		out_cont(0xA7);
		syscall_monitor_write("   Two Chanels = YES\n");
	} else {
		syscall_monitor_write("   Two Chanels = NO");
	}
}

void interface_test() {	//6
	syscall_monitor_write("6. checking peripherals\n");
	out_cont(0xAB);
	has_key_pressed();
	u8int e = in();
	if (e == 0x00) {syscall_monitor_write("   PS/2_1 = GOOD\n");PS2_1 = 1;}
	if (e == 0x01) {syscall_monitor_write("   PS/2_1 = ClOCK STUCK LOW\n");PS2_1 = 0;}
	if (e == 0x02) {syscall_monitor_write("   PS/2_1 = CLOCK STUCK HIGHT\n");PS2_1 = 0;}
	if (e == 0x03) {syscall_monitor_write("   PS/2_1 = DATA STUCK LOW\n");PS2_1 = 0;}
	if (e == 0x04) {syscall_monitor_write("   PS/2_1 = DATA STUCK HIGH\n");PS2_1 = 0;}
	
	if(PS2_2) {
		out_cont(0xA9);
		has_key_pressed();
		u8int e = in();
		if (e == 0x00) {syscall_monitor_write("   PS/2_2 = GOOD\n");PS2_2 = 1;}
		if (e == 0x01) {syscall_monitor_write("   PS/2_2 = ClOCK STUCK LOW\n");PS2_2 = 0;}
		if (e == 0x02) {syscall_monitor_write("   PS/2_2 = CLOCK STUCK HIGHT\n");PS2_2 = 0;}
		if (e == 0x03) {syscall_monitor_write("   PS/2_2 = DATA STUCK LOW\n");PS2_2 = 0;}
		if (e == 0x04) {syscall_monitor_write("   PS/2_2 = DATA STUCK HIGH\n");PS2_2 = 0;} 
	}
}

void enable_devices() {	//7
	syscall_monitor_write("7. enabling devices...\n");
	config = in_config();
	if(PS2_1) {
		out_cont(0xAE);
		config |= 1 << 0;
	}
	/*if(PS2_2) {
		out_cont(0xA8);
		config |= 1 << 1;
	}*/
}

void reset_devices() {	//8
	syscall_monitor_write("8. resetting devices\n");
	if (PS2_1) {
		outb(0x60, 0xFF);
		has_key_pressed();
		u8int resp = inb(0x60);
		if (resp == 0xFA) {syscall_monitor_write("   PS/2_1 = GOOD\n");}
		if (resp == 0xFC) {syscall_monitor_write("   PS/2_1 = FAIL\n");}
	}
	/*if (PS2_2) {
		outb(0x64, 0xD4);
		outb(0x60, 0xFF);
		u8int resp;
		u16int p = 0;
		has_key_pressed();
		resp = inb(0x60);
		resp = inb(0x60);
		if (resp == 0xFA) {syscall_monitor_write("   PS/2_2 = GOOD\n");}
		if (resp == 0xFC) {syscall_monitor_write("   PS/2_2 = FAIL\n");}
	}*/
	
}

void lable_devices() {	//9
	syscall_monitor_write("9. getting device names\n");
	if(PS2_1) {
		out_key(0xF5);
		has_key_pressed();
		while(in() != 0xFA) {}
		out_key(0xF2);
		has_key_pressed();
		while(in() != 0xFA) {}
		u8int dev[2];
		dev[0] = inb(0x60);
		dev[1] = inb(0x60);
		if(dev[0] == 0x00) {syscall_monitor_write("[INFO] [Mouse] [PS/2 1] Standard PS/2 Mouse\n");}
		if(dev[0] == 0x03) {syscall_monitor_write("[INFO] [Mouse] [PS/2 1] Mouse w/ scroll wheel\n");}
		if(dev[0] == 0x04) {syscall_monitor_write("[INFO] [Mouse] [PS/2 1] 5-Button Mouse\n");}
		if(dev[0] == 0xAB && dev[1] == 0x41 || dev[0] == 0xAB && dev[1] == 0xC1) {syscall_monitor_write("[INFO] [Keyboard] [PS/2 1] MF2 Keyboard(translation)\n");}
		if(dev[0] == 0xAB && dev[1] == 0x83) {syscall_monitor_write("[INFO] [Keyboard] [PS/2 1] MF2 Keyboard\n");}
	}
	
	/*if(PS2_2) {
		out_cont(0xD4);
		out_key(0xF5);
		has_key_pressed();
		while(in() != 0xFA) {}
		out_cont(0xD4);
		out_key(0xF2);
		has_key_pressed();
		while(in() != 0xFA) {}
		u8int dev[2];
		dev[0] = inb(0x60);
		dev[1] = inb(0x60);
		if(dev[0] == 0x00) {syscall_monitor_write("[INFO] [Mouse] [PS/2 2] Standard PS/2 Mouse\n");}
		if(dev[0] == 0x03) {syscall_monitor_write("[INFO] [Mouse] [PS/2 2] Mouse w/ scroll wheel\n");}
		if(dev[0] == 0x04) {syscall_monitor_write("[INFO] [Mouse] [PS/2 2] 5-Button Mouse\n");}
		if(dev[0] == 0xAB && dev[1] == 0x41 || dev[0] == 0xAB && dev[1] == 0xC1) {syscall_monitor_write("[INFO] [Keyboard] [PS/2 2] MF2 Keyboard(translation)\n");}
		if(dev[0] == 0xAB && dev[1] == 0x83) {syscall_monitor_write("[INFO] [Keyboard] [PS/2 2] MF2 Keyboard\n");}
	}*/
}

u8int check_command_buffer() {
	if(COMMAND_BUFFER[0].full == 1) {
		return 1;
	} else return 0;
}

void keyboard_handler(registers_t *regs) {
		//BOOL_KEY_PRESSED = 1;
		syscall_monitor_write("k: ");
		if(check_command_buffer()) {
		
		}
		
		
		}
