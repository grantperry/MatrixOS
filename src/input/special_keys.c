#include "special_keys.h"
#include "../common.h"

void doSpecial ( u16int i ) {
	if ( i == 1 ) {
		VGA_init ( 320, 200, 256 ); //dont change these numbers
	}

	if ( i == 12 ) {
		colour_fun();
	}
}
