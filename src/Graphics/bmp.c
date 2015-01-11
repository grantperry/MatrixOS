#include <stdio.h>
#include <Graphics/Graphics.h>

u32int openBMP(char *name) {
	FILE *bmp;
	bmp = ( FILE* ) f_open ( name, fs_root, "r" );
	serialf ( "BMP: %s\nBMP: size: %h\n", name, bmp->size );
	u8int *bmpbuf;// = ( u8int* ) kmalloc ( bmp->size );
	serialf("BMP: Memalloc: DONE!\n");
	//f_read ( bmp, 0, bmp->size - 1, bmpbuf );
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
			
			u32int offset = ( y * (*pixelBPP / 8) + ((x * 3) * ye ));
			
			
			r = *(bmpbuf + *pixelArayOff + offset + 0);
			g = *(bmpbuf + *pixelArayOff + offset + 1);
			b = *(bmpbuf + *pixelArayOff + offset + 2);
			u32int c = (b << 16) + (g << 8) + (r << 0);
			//u32int c = (r << 16) + (g << 8) + (b << 0);
			drawPixel(y, xe-x, c);
			//serialf("x:%d y:%d  %h %h %h  %h    %h\n", x, y, r, g, b, c, offset);
			y++;
		}
		x++;
	}
	serialf("END\n");
	
	
	return 0;
}
