#include "graphics.h"

unsigned char *VGA = (unsigned char *)0xA0000000;

void init_graphics() {
	while(1) {
		&VGA = 1;
	}
}
