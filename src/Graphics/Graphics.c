#include <stdio.h>
#include <Graphics/Graphics.h>
#include <VESA/VESA.h>
#include <paging.h>

u32int DispX = 0;
u32int DispY = 0;
u32int DispD = 0;
u32int VESAMemLoc = 0;

u8int *vga_mem = 0;

void (*drawPixel)(u32int X, u32int Y, u32int colour);

void drawPixelVesa24(u32int X, u32int Y, u32int colRGB) {
	int offset = X * (DispD / 8) + Y * (DispX * (DispD / 8));
	//serialf("off: %d\n", offset);
	*(vga_mem + offset)     = colRGB & 0xff;           //BLUE
	*(vga_mem + offset + 1) = (colRGB >> 8) & 0xff;    //GREEN
	*(vga_mem + offset + 2) = (colRGB >> 16) & 0xff;   //RED
}

void drawRect(u32int X, u32int Y, u32int Xlen, u32int Ylen, u32int RBG) {
	u32int a,b;
	for (a = X; a <= (X + Xlen); a++) {
		for (b = Y; b <= (Y + Ylen); b++) {
			drawPixel(a, b, RBG);
		}
	}
}

void setDisplay(u32int mode) {
	setVesa(mode);
	serialf("Starting Graphics at Dimensions\n\tX: %d\n\tY: %d\n\tD: %d\n\tMemory Location(Phys): %h\n", DispX, DispY, DispD, VESAMemLoc);
	serialf("s: %h\n", (DispX*DispY*(DispD/8)));
	virtual_map_pages(VESAMemLoc, (DispX*DispY*(DispD/8)), 1, 1);
	vga_mem = (u8int*) VESAMemLoc;
	drawPixel = (void*)drawPixelVesa24;
	
}

