#include "../common.h"
#include "i8042.h"
#include "../timer.h"

#define I8042_KBD_PORT_NO	0
#define I8042_AUX_PORT_NO	1

volatile s32int shift_flag = 0;
volatile s32int caps_flag = 0;

//Read data port(0x60) and return value
static inline int i8042_read_data(void)
{
	return inb(I8042_DATA_REG);
}

//Read status port(0x64) and return value
static inline int i8042_read_status(void)
{
	return inb(I8042_STATUS_REG);
}

//Write value to data port(0x60)
static inline void i8042_write_data(int val)
{
	outb(val, I8042_DATA_REG);
}

//Write value to command port(0x64)
static inline void i8042_write_command(int val)
{
	outb(val, I8042_COMMAND_REG);
}

////////////////////////////////////////////////////////
void i8042_disable_devices() {	//1
	syscall_monitor_write("\n1. disabling devices...\n");
	i8042_write_command(0xAD);
	i8042_write_command(0xA7);
}

void i8042_flush_output_buffer() {	//2
	syscall_monitor_write("2. flushing output buffer\n");
	
}

void i8042_set_controller_config_byte() {	//3
	syscall_monitor_write("3. setting controller config byte\n");

}

void i8042_controller_self_test() {	//4
	syscall_monitor_write("4. controller self test\n");

}

void i8042_two_channels() {	//5
	syscall_monitor_write("5. finding channels\n");

}

void i8042_interface_test() {	//6
	syscall_monitor_write("6. checking peripherals\n");


}

void i8042_enable_devices() {	//7
	syscall_monitor_write("7. enabling devices...\n");

}

void i8042_reset_devices() {	//8
	syscall_monitor_write("8. resetting devices\n");

}

void i8042_lable_devices() {	//9
	syscall_monitor_write("9. name devices\n");

}

