#include "../common.h"
#include "i8042.h"
#include "../timer.h"

#define I8042_KBD_PORT_NO	0
#define I8042_AUX_PORT_NO	1

#define DEBUG				0

u8int status = 0;


u8int PS2_2 = 0;
u8int PS2_1 = 0;

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
u8int i8042_disable_devices() {	//1
	if(DEBUG) syscall_monitor_write("disabling devices...");
	i8042_write_command(0xAD);
	i8042_write_command(0xA7);
	return 0;
}

u8int i8042_flush_output_buffer() {	//2
	if(DEBUG) syscall_monitor_write("flushing output buffer");
	u8int i = 0;
	u8int c;
	while(i < I8042_BUFFER_SIZE) {
		c = i8042_read_data();
		i++;
	}
	return 0;
}

u8int i8042_set_controller_config_byte() {	//3
	if(DEBUG) syscall_monitor_write("setting controller config byte");
	i8042_write_command(0x20);
	status = i8042_read_data();
	status |= 1 << 0;
	status |= 1 << 1;
	status |= 1 << 6;
	i8042_write_command(0x60);
	i8042_write_data(status);
	if(DEBUG && !(status & (1 << 5))) syscall_monitor_write("DUAL PS/2 = YES\n"); //TODO check if this is right
	return 0;
}

u8int i8042_controller_self_test() {	//4
	if(DEBUG) syscall_monitor_write("controller self test");
	i8042_write_command(0xAA);
	u8int resp = i8042_read_data();
	resp = i8042_read_data();
	if(DEBUG) {
		if (resp == 0x55) syscall_monitor_write("PASS\n");
		if (resp == 0xFC) syscall_monitor_write("FAIL\n");
	}
	return 0;
}

u8int i8042_two_channels() {	//5
	if(DEBUG) syscall_monitor_write("finding channels");
	return 0;
}

u8int i8042_interface_test() {	//6
	if(DEBUG) syscall_monitor_write("checking peripherals");
	return 0;
	
}

u8int i8042_enable_devices() {	//7
	if(DEBUG) syscall_monitor_write("enabling devices...");
	i8042_write_command(0xAE);
	
	i8042_write_command(0x20);
	u8int stat = i8042_read_data();
	stat |= 1 << 0;
	stat |= 1 << 1;
	i8042_write_command(0x60);
	i8042_write_data(stat);
	return 0;
}

u8int i8042_reset_devices() {	//8
	if(DEBUG) syscall_monitor_write("resetting devices");
	return 0;
}

u8int i8042_lable_devices() {	//9
	if(DEBUG) syscall_monitor_write("name devices");
	return 0;
}

