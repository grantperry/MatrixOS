#include "keyboard.h"
#include "i8042.h"
#include "../monitor.h"
#include "../isr.h"
#include "../common.h"
#include "special_keys.h"

#include "../fun/fun.h"
#include "../system/moduleLoading.h"

u8int shift_flag = 0;
u8int CapsOn = 0, NumOn = 0, ScrollOn = 0;

volatile char currentKey = 0;

char getKey() {
	char currKey = currentKey;
	currentKey = 0;
	return currKey;
}

s8int init_keyboard() { /////////////////////////////////////////////////////////
	syscall_monitor_write ( "Initalizing Keyboard." );
	register_interrupt_handler ( IRQ1, &keyboard_handler );
	i8042_disable_devices();
	i8042_flush_output_buffer();
	i8042_set_controller_config_byte();
	i8042_controller_self_test();
	i8042_two_channels();
	i8042_interface_test();
	i8042_enable_devices();
	i8042_reset_devices();
	i8042_lable_devices();
	return 0;
}

s32int isSpecialKey ( unsigned char keyPressChar ) {
	switch ( keyPressChar ) {
	case 'K': //scancode for left arrow key
		return 1;

	case 'M': //scancode for right arrow key
		return 2;

	case 'H': //scancode for up arrow key
		return 3;

	case 'P': //scancode for down arrow key
		return 4;

	case 0x1C:
		return 5;

	case 0x58: /* F12 */
		doSpecial ( 12 );
		return -1;

	case 0x3B: /* F1 */
		doSpecial ( 1 );
		return -1;

	default:
		return 0;
	}
}

char lowerCaseKbdus[128] = {
	0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
	'9', '0', '-', '=', '\b',	/* Backspace */
	'\t',			/* Tab */
	'q', 'w', 'e', 'r',	/* 19 */
	't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0,		/* Enter key */
	0,			/* 29   - Control */
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
	'\'', '`',   0,		/* Left shift */
	'\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
	'm', ',', '.', '/',   'r',					/* Right shift */
	'*',
	0,	/* Alt */
	' ',	/* Space bar */
	0,	/* Caps lock */
	0,	/* 59 - F1 key ... > */
	0,   0,   0,   0,   0,   0,   0,   0,
	0,	/* < ... F10 */
	0,	/* 69 - Num lock*/
	0,	/* Scroll Lock */
	0,	/* Home key */
	0,	/* Up Arrow */
	0,	/* Page Up */
	'-',
	0,	/* Left Arrow */
	0,
	0,	/* Right Arrow */
	'+',
	0,	/* 79 - End key*/
	0,	/* Down Arrow */
	0,	/* Page Down */
	0,	/* Insert Key */
	0,	/* Delete Key */
	0,   0,   0,
	0,	/* F11 Key */
	0,	/* F12 Key */
	0,	/* All other keys are undefined */
};

char upperCaseKbdus[128] = {
	0,  27, '!', '@', '#', '$', '%', '^', '&', '*',	/* 9 */
	'(', ')', '_', '+', '\b',	/* Backspace */
	'\t',			/* Tab */
	'Q', 'W', 'E', 'R',	/* 19 */
	'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\r',		/* Enter key */
	0,			/* 29   - Control */
	'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',	/* 39 */
	'\"', '~',   0,		/* Left shift */
	'|', 'Z', 'X', 'C', 'V', 'B', 'N',			/* 49 */
	'M', '<', '>', '?',   0,					/* Right shift */
	'*',
	0,	/* Alt */
	' ',	/* Space bar */
	0,	/* Caps lock */
	0,	/* 59 - F1 key ... > */
	0,   0,   0,   0,   0,   0,   0,   0,
	0,	/* < ... F10 */
	0,	/* 69 - Num lock*/
	0,	/* Scroll Lock */
	0,	/* Home key */
	0,	/* Up Arrow */
	0,	/* Page Up */
	'-',
	0,	/* Left Arrow */
	0,
	0,	/* Right Arrow */
	'+',
	0,	/* 79 - End key*/
	0,	/* Down Arrow */
	0,	/* Page Down */
	0,	/* Insert Key */
	0,	/* Delete Key */
	0,   0,   0,
	0,	/* F11 Key */
	0,	/* F12 Key */
	0,	/* All other keys are undefined */
};

void setKey(char c) {
	currentKey = c;
}


void keyboard_handler ( registers_t *regs ) {
	u8int scancode = inb ( 0x60 );
	u8int specialKey = 0;
	specialKey = isSpecialKey ( scancode );
	//currentKey = 0;

	if ( scancode & 0x80 ) {
		scancode = scancode - 0x80;

		if ( scancode == 42 || scancode - 0x80 == 54 ) {
			shift_flag = 0;
		}

	} else {
		if ( scancode == 42 || scancode - 0x80 == 54 ) {
			shift_flag = 1;
		}

		if ( scancode == 0x3A ) {
			CapsOn = !CapsOn;
		}

		if ( shift_flag == 0 && CapsOn == 0 ) {
			//monitor_put ( lowerCaseKbdus[scancode] );
			setKey(lowerCaseKbdus[scancode]); //set Current Key for key logger.
		}

		if ( shift_flag == 1 || CapsOn == 1 ) {
			//monitor_put ( upperCaseKbdus[scancode] );
			setKey(upperCaseKbdus[scancode]); //set Current Key for key logger.
		}
		
		monitor_put(currentKey);

		if ( specialKey != 0 ) {
			switch ( specialKey ) {
			case 1:
				monitor_command ( "cursor", "left" );
				break;

			case 2:
				monitor_command ( "cursor", "right" );
				break;

			case 3:
				monitor_command ( "cursor", "up" );
				break;

			case 4:
				monitor_command ( "cursor", "down" );
				break;

			case 5:
				monitor_put ( '\r' );
				break;

			default:
				break;
			}

		}
	}
}
