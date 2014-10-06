#include "../common.h"
#include "i8042.h"
#include "keyboard.h"
#include "../timer.h"

#define I8042_KBD_PORT_NO	0
#define I8042_AUX_PORT_NO	1

#define DEBUG_KBD			0

u8int status = 0;

u8int ltmp = 0;

u8int PS2_2 = 0;
u8int PS2_1 = 0;

//Read data port(0x60) and return value
static inline int i8042_read_data ( void ) {
	return inb ( I8042_DATA_REG );
}

//Read status port(0x64) and return value
static inline int i8042_read_status ( void ) {
	return inb ( I8042_STATUS_REG );
}

//Write value to data port(0x60)
static inline void i8042_write_data ( int val ) {
	outb ( val, I8042_DATA_REG );
}

//Write value to command port(0x64)
static inline void i8042_write_command ( int val ) {
	outb ( val, I8042_COMMAND_REG );
}

////////////////////////////////////////////////////////

/*
// Initialize the i8042 controller
*/
u8int i8042_Init() { 
	i8042_disable_devices();
	i8042_flush_output_buffer();
	i8042_set_controller_config_byte();
	i8042_controller_self_test();
	i8042_two_channels();
	i8042_interface_test();
	i8042_enable_devices();
	i8042_reset_devices();
	i8042_lable_devices();
	
	inb(0x60);
	outb(0x60, 0xF3);
	while(!(inb(0x60) == 0xFA));
	outb(0x60, 1 << 5); //set the typematic rate to top
	inb(0x60);
	inb(0x60);
}

void i8042_Caps(u8int Caps, u8int Scroll, u8int Num) {
	inb(0x60);			//
	outb(0x60, 0xED);	//
	inb(0x60);			//We need to turn all of them off before changing them.
	outb(0x60, 0);		//
	inb(0x60);			//
	inb(0x60);
	
	ltmp = 0;
	
	outb(0x60,0xED);
	while(!(inb(0x60) == 0xFA));
	if (Caps) {
		ltmp |= 4; // bit 3
	}
	if (Scroll) {
		ltmp |= 1; // bit 1
	}
	if (Num) {
		ltmp |= 2; // bit 2
	}
	outb(0x60,ltmp);
	if(!(inb(0x60) == 0xFA)) {/*Fail*/}
	inb(0x60);
	inb(0x60);
}

u8int i8042_disable_devices() {	//1
	if ( DEBUG_KBD ) {
		printf ( "disabling devices..." );
	}

	i8042_write_command ( 0xAD );
	i8042_write_command ( 0xA7 );
	return 0;
}

u8int i8042_flush_output_buffer() {	//2
	if ( DEBUG_KBD ) {
		printf ( "flushing output buffer" );
	}

	u8int i = 0;
	u8int c;

	while ( i < I8042_BUFFER_SIZE ) {
		c = i8042_read_data();
		i++;
	}

	return 0;
}

u8int i8042_set_controller_config_byte() {	//3
	if ( DEBUG_KBD ) {
		printf ( "setting controller config byte" );
	}

	i8042_write_command ( 0x20 );
	status = i8042_read_data();
	status |= 1 << 0;
	status |= 1 << 1;
	status |= 1 << 6;
	i8042_write_command ( 0x60 );
	i8042_write_data ( status );

	if ( DEBUG_KBD && ! ( status & ( 1 << 5 ) ) ) {
		printf ( "DUAL PS/2 = YES\n" );    //TODO check if this is right
	}

	return 0;
}

u8int i8042_controller_self_test() {	//4
	if ( DEBUG_KBD ) {
		printf ( "controller self test" );
	}

	i8042_write_command ( 0xAA );
	u8int resp = i8042_read_data();
	resp = i8042_read_data();

	if ( DEBUG_KBD ) {
		if ( resp == 0x55 ) {
			printf ( "PASS\n" );
		}

		if ( resp == 0xFC ) {
			printf ( "FAIL\n" );
		}
	}

	return 0;
}

u8int i8042_two_channels() {	//5
	if ( DEBUG_KBD ) {
		printf ( "finding channels" );
	}

	return 0;
}

u8int i8042_interface_test() {	//6
	if ( DEBUG_KBD ) {
		printf ( "checking peripherals" );
	}

	return 0;

}

u8int i8042_enable_devices() {	//7
	if ( DEBUG_KBD ) {
		printf ( "enabling devices..." );
	}

	i8042_write_command ( 0xAE );

	i8042_write_command ( 0x20 );
	u8int stat = i8042_read_data();
	stat |= 1 << 0;
	stat |= 1 << 1;
	i8042_write_command ( 0x60 );
	i8042_write_data ( stat );
	return 0;
}

u8int i8042_reset_devices() {	//8
	if ( DEBUG_KBD ) {
		printf ( "resetting devices" );
	}

	return 0;
}

u8int i8042_lable_devices() {	//9
	if ( DEBUG_KBD ) {
		printf ( "name devices" );
	}

	return 0;
}

