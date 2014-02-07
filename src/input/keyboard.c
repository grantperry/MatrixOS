#include "keyboard.h"
#include "../isr.h"
#include "../common.h"

void disable_devices() {	//1
	syscall_monitor_write("\n1. disabling devices...\n");
	
}

void flush_output_buffer() {	//2
	syscall_monitor_write("2. flushing output buffer\n");
	
}

void set_controller_config_byte() {	//3
	syscall_monitor_write("3. setting controller config byte\n");

}

void controller_self_test() {	//4
	syscall_monitor_write("4. controller self test\n");

}

void two_channels() {	//5
	syscall_monitor_write("5. finding channels\n");

}

void interface_test() {	//6
	syscall_monitor_write("6. checking peripherals\n");


}

void enable_devices() {	//7
	syscall_monitor_write("7. enabling devices...\n");

}

void reset_devices() {	//8
	syscall_monitor_write("8. resetting devices\n");

}

void lable_devices() {	//9
	syscall_monitor_write("8. name devices\n");

}

void init_keyboard() { /////////////////////////////////////////////////////////
	register_interrupt_handler(IRQ1, &keyboard_handler);
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

void keyboard_handler(registers_t *regs) {
	//BOOL_KEY_PRESSED = 1;
	syscall_monitor_write("k: ");
}
