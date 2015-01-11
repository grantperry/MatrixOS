#include <stdio.h>
#include <Graphics/Graphics.h>

u32int openBMP(char *name) {
	FILE *bmp;
	bmp = ( FILE* ) f_open ( name, fs_root, "r" );
	serialf ( "BMP: %s\n", name );
	u8int *bmpbuf = ( u8int* ) kmalloc ( 0x5FFFF );
	serialf("BMP: Memalloc: DONE!\n");
	f_read ( bmp, 0, 0x5FFFF, bmpbuf );
	serialf("BMP: Read:     DONE!\n");
	
	u16int *magic = (u16int*)bmpbuf;
	serialf("Magic No.:     %h\n", *magic);
	u32int *bmpFSize = (u32int*)(bmpbuf + 0x02);
	serialf("File Size:     %h\n", *bmpFSize);
	u32int *pixelArayOff = (u32int*)(bmpbuf + 0x0A);
	serialf("Pixel Ary OFF: %h\n", *pixelArayOff);
	u32int *pixelWide = (u32int*)(bmpbuf + 0x12);
	serialf("Pixel's Wide:  %h\n", *pixelWide);
	u32int *pixelTall = (u32int*)(bmpbuf + 0x16);
	serialf("Pixel's Tall:  %h\n", *pixelTall);
	u32int *pixelBPP = (u32int*)(bmpbuf + 0x1C);
	serialf("Pixel BPP:     %h\n", *pixelBPP);
	
	u32int x = 0,y = 0;
	u32int xe, ye;
	xe = *pixelWide;
	ye = *pixelTall;
	serialf("xe:%d ye:%d\n", xe, ye);
	while(x <= xe) {
		y = 0;
		while(y <= ye) {
			u8int r,g,b;
			u32int offset = ((x * (*pixelBPP/8)) + (y * xe));
			r = *(bmpbuf + *pixelArayOff + offset + 0);
			g = *(bmpbuf + *pixelArayOff + offset + 1);
			b = *(bmpbuf + *pixelArayOff + offset + 2);
			u32int c = (r << 16) + (g << 8) + b;
			drawPixel(xe-x, ye-y, c);
			//serialf("x:%d y:%d  %h %h %h  %h\n", x, y, r, g, b, c);
			y++;
		}
		x++;
	}
	serialf("END\n");
	
	
	return 0;
}
