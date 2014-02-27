#include "graphics.h"
#include "../common.h"

unsigned char* vram = (unsigned char*)0xA0000;

void VGA_init (int width, int height, int bpp) {
	k_save();
	regs16_t regs;
	regs.ax = 0x0013;
	int32(0x10, &regs);
}
