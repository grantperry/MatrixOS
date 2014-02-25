/*#include "graphics.h"
#include "../common.h"

u8int *double_buffer; //double buffer for VESA video
s32int isVESAon;

void VGA_init(int width, int height, int bpp)
{
	//set the colors for the VGA graphics modes
	outb(0x3c8, 0x0f);
	outb(0x3c9, 0x3f);
	outb(0x3c9, 0x3f);
	outb(0x3c9, 0x3f);
   
	VGA_width = (unsigned int)width;
	VGA_height = (unsigned int)height;
	VGA_bpp = bpp;
  //~ VGA_address = (unsigned char *)0xA0000;

	if(width == 1024 && height == 768 && bpp == 24)
  {
    double_buffer = (u8int*)kmalloc_a((width * height) * (bpp / 8));

    //~ k_printf("\ndouble buffer: %h\n", double_buffer);
    
    memset(double_buffer, 0xff, width * height * (bpp / 8));

    //~ k_printf("double buffer content: %h %h %h %h %h %h %h\n", double_buffer[0], double_buffer[1], double_buffer[2], double_buffer[3], double_buffer[4], double_buffer[5]);
    //~ k_printf("Double buffer addresses: %h %h %h %h %h %h %h %h\n", &double_buffer[0], &double_buffer[1], &double_buffer[2], &double_buffer[3], &double_buffer[4], &double_buffer[5], &double_buffer[6]);

    //~ while(1);

    isVESAon = ON;
    setVesa(0x118); //1024x768x24
    putPixel = putPixel_VESA;
  
}*/
