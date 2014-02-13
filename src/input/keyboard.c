#include "keyboard.h"
#include "i8042.h"
#include "../isr.h"
#include "../common.h"


void init_keyboard() { /////////////////////////////////////////////////////////
	register_interrupt_handler(IRQ1, &keyboard_handler);
	i8042_disable_devices();
	i8042_flush_output_buffer();
	i8042_set_controller_config_byte();
	i8042_controller_self_test();
	i8042_two_channels();
	i8042_interface_test();
	i8042_enable_devices();
	i8042_reset_devices();
	i8042_lable_devices();
}

void keyboard_handler(registers_t *regs) {
	u8int scancode = inb(0x60);
	if(scancode & 0x80) {
		syscall_monitor_write("release\n");
	}else{syscall_monitor_write("press\n");}
}
