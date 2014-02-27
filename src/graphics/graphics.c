#include "graphics.h"
#include "../common.h"

//the vga identifiers
u32int VGA_width;
u32int VGA_height;
u32int VGA_bpp;

void (*putPixel)(int, int, int);

unsigned char* vram = (unsigned char*)0xA0000;

void putPixel_simpleStd(int x, int y, int color)
{
	int offset = x + VGA_width * y;
	vram[offset] = color;
}

void putRect(int x, int y, int width, int height, int fill)
{
  int offset, tmpX = x, tmpY = y;
  
  for(y = tmpY; y < height + tmpY; y++)
    for(x = tmpX; x < width + tmpX; x++)
      putPixel(x, y, fill);
      
}

void VGA_init (int width, int height, int bpp) {
	k_save();
	VGA_width = width;
	VGA_height = height;
	VGA_bpp = bpp;
	regs16_t regs;
	regs.ax = 0x0013;
	int32(0x10, &regs);
	putPixel = putPixel_simpleStd;
	putRect(10,10,100,100,2);
	putRect(20,20,80,80,3);
}
