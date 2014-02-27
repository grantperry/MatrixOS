#include "graphics.h"
#include "../common.h"

u8int *double_buffer; //double buffer for VESA video
s32int isVESAon;

extern u8int *vga_mem; //pointer to VESA Linear Frame Buffer
extern u32int widthVESA, heightVESA, depthVESA; //size of VESA Screen attributes
void (*putPixel)(int, int, int);
void putPixel_VESA(int x, int y, int RGB);

//the vga identifiers
u32int VGA_width;
u32int VGA_height;
u32int VGA_bpp;

static void vsync()
{
  /*wait until any previous retrace has ended*/
  do {
  } while (inb(0x3DA) & 8);
  
  /*wait until a new retrace has just begun*/
  do {
  } while (!(inb(0x3DA) & 8));
}

static void g_flip(unsigned char *source, u32int count)
{
  memcpy(vga_mem, source, count);
}

void refreshScreen_VESA()
{
  vsync();

  //set the double buffer to the physical graphics memmory (vga_mem)
  g_flip(double_buffer, (widthVESA * heightVESA * (depthVESA / 8)));
  
}

void putRect(int x, int y, int width, int height, int fill)
{
  int offset, tmpX = x, tmpY = y;
  
  for(y = tmpY; y < height + tmpY; y++)
    for(x = tmpX; x < width + tmpX; x++)
      putPixel(x, y, fill);
      
}

void go() {
	putRect(50, 50, 100, 500, 100);
	refreshScreen_VESA();
}

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
  syscall_monitor_write("1024x768x24 VESA\n");
  sleep(1);
    double_buffer = (u8int*)kmalloc_a((width * height) * (bpp / 8));

    syscall_monitor_write("\nDouble buffer: ");
    syscall_monitor_write_hex(double_buffer);
        
    memset(double_buffer, 0xff, width * height * (bpp / 8));

    syscall_monitor_write("\nDouble buffer content: ");
    syscall_monitor_write_hex(double_buffer[0]);
    syscall_monitor_write(" ");
    syscall_monitor_write_hex(double_buffer[1]);
    syscall_monitor_write(" ");
    syscall_monitor_write_hex(double_buffer[2]);
    syscall_monitor_write(" ");
    syscall_monitor_write_hex(double_buffer[3]);
    syscall_monitor_write(" ");
    syscall_monitor_write_hex(double_buffer[4]);
    syscall_monitor_write(" ");
    syscall_monitor_write_hex(double_buffer[5]);
    syscall_monitor_write(" ");
    
    syscall_monitor_write("\nDouble buffer addresses: ");
    syscall_monitor_write_hex(&double_buffer[0]);
    syscall_monitor_write(" ");
    syscall_monitor_write_hex(&double_buffer[1]);
    syscall_monitor_write(" ");
    syscall_monitor_write_hex(&double_buffer[2]);
    syscall_monitor_write(" ");
    syscall_monitor_write_hex(&double_buffer[3]);
    syscall_monitor_write(" ");
    syscall_monitor_write_hex(&double_buffer[4]);
    syscall_monitor_write("\n");
    syscall_monitor_write_hex(&double_buffer[5]);

    //~ while(1);

    isVESAon = ON;
    //setVesa(0x118); //1024x768x24
    putPixel = putPixel_VESA;
}
}

void putPixel_VESA(int x, int y, int RGB)
{
	//syscall_monitor_write("putpix\n");
  /*calculates the offset for a specific point on screen*/
  int offset = x * (depthVESA / 8) + y * (widthVESA * (depthVESA / 8));

  vga_mem[offset + 0] = RGB & 0xff;           //BLUE
  vga_mem[offset + 1] = (RGB >> 8) & 0xff;    //GREEN
  vga_mem[offset + 2] = (RGB >> 16) & 0xff;   //RED

}
