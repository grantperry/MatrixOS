#include "graphics.h"
#include "../common.h"
#include "font.h"
#include "../math/math.h"

//the vga identifiers
u32int VGA_width;
u32int VGA_height;
u32int VGA_bpp;

extern u8int *vga_mem; //pointer to VESA Linear Frame Buffer
extern u8int *double_buffer; //double buffer for VESA video

void ( *putPixel ) ( int, int, int );
extern int font_std[128][8];

extern u32int widthVESA, heightVESA, depthVESA; //size of VESA Screen attributes

u8int isVESAon = 0;
u8int *double_buffer; //double buffer for VESA video

u16int chars_wide, chars_hight, chars_pos, chars_size;

unsigned char* vram = ( unsigned char* ) 0xA0000;

void putPixel_simpleStd ( int x, int y, int color ) {
	int offset = x + VGA_width * y;
	vram[offset] = color;
}

void putPixel_VESA(int x, int y, int RGB)
{

  /*calculates the offset for a specific point on screen*/
  int offset = x * (depthVESA / 8) + y * (widthVESA * (depthVESA / 8));

  vga_mem[offset + 0] = RGB & 0xff;           //BLUE
  vga_mem[offset + 1] = (RGB >> 8) & 0xff;    //GREEN
  vga_mem[offset + 2] = (RGB >> 16) & 0xff;   //RED

}

void putRect ( int x, int y, int width, int height, int fill ) {
	int offset, tmpX = x, tmpY = y;

	for ( y = tmpY; y < height + tmpY; y++ )
		for ( x = tmpX; x < width + tmpX; x++ ) {
			putPixel ( x, y, fill );
		}

}

//define the ports , taken from http://files.osdev.org/mirrors/geezer/osd/graphics/modes.c
#define   VGA_AC_INDEX             0x3C0
#define   VGA_AC_WRITE             0x3C0
#define   VGA_AC_READ              0x3C1
#define   VGA_MISC_WRITE           0x3C2
#define   VGA_SEQ_INDEX            0x3C4
#define   VGA_SEQ_DATA             0x3C5
#define   VGA_DAC_READ_INDEX       0x3C7
#define   VGA_DAC_WRITE_INDEX      0x3C8
#define   VGA_DAC_DATA             0x3C9
#define   VGA_MISC_READ            0x3CC
#define   VGA_GC_INDEX             0x3CE
#define   VGA_GC_DATA              0x3CF
#define   VGA_CRTC_INDEX           0x3D4      /* 0x3B4 */
#define   VGA_CRTC_DATA            0x3D5      /* 0x3B5 */
#define   VGA_INSTAT_READ          0x3DA
#define   VGA_NUM_SEQ_REGS         5
#define   VGA_NUM_CRTC_REGS        25
#define   VGA_NUM_GC_REGS          9
#define   VGA_NUM_AC_REGS          21
#define   VGA_NUM_REGS      (1+VGA_NUM_SEQ_REGS+VGA_NUM_CRTC_REGS+VGA_NUM_GC_REGS+VGA_NUM_AC_REGS)

unsigned char mode_320_200_256[]= {
	/* MISC
	 *
	 * 0x63 => 01100011
	 * 7 6 5 4 3 2 1 0
	 * 1 1 0 0 0 1 1 0
	 * VSP HSP - - CS CS ERAM IOS
	 * 7,6 - 480 lines
	 * 5,4 - free
	 * 3,2 - 28,322 MHZ Clock
	 * 1 - Enable Ram
	 * 0 - Map 0x3d4 to 0x3b4
	 */
	0x63,
	/* SEQ */
	/**
	 * index 0x00 - Reset
	 * 0x03 = 11
	 * Bits 1,0 Synchronous reset
	 */
	0x03,
	/**
	 * index 0x01
	 * Clocking mode register
	 * 8/9 Dot Clocks
	 */
	0x01,
	/**
	 * Map Mask Register, 0x02
	 * 0x0F = 1111
	 * Enable all 4 Maps Bits 0-3
	 * chain 4 mode
	 */
	0x0F,
	/**
	 * map select register, 0x03
	 * no character map enabled
	 */
	0x00,
	/**
	 * memory mode register 0x04
	 * enables ch4,odd/even,extended memory
	 */
	0x0E,
	/* CRTC */
	0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
	0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x9C, 0x0E, 0x8F, 0x28,   0x40, 0x96, 0xB9, 0xA3,
	0xFF,
	/* GC */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
	0xFF,
	/* AC */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x41, 0x00, 0x0F, 0x00,   0x00
};

void write_registers ( unsigned char *regs ) {
	unsigned i;


	/* write MISCELLANEOUS reg */
	outb ( VGA_MISC_WRITE, *regs );
	regs++;

	/* write SEQUENCER regs */
	for ( i = 0; i < VGA_NUM_SEQ_REGS; i++ ) {
		outb ( VGA_SEQ_INDEX, i );
		outb ( VGA_SEQ_DATA, *regs );
		regs++;
	}

	/* unlock CRTC registers */
	outb ( VGA_CRTC_INDEX, 0x03 );
	outb ( VGA_CRTC_DATA, inb ( VGA_CRTC_DATA ) | 0x80 );
	outb ( VGA_CRTC_INDEX, 0x11 );
	outb ( VGA_CRTC_DATA, inb ( VGA_CRTC_DATA ) & ~0x80 );
	/* make sure they remain unlocked */
	regs[0x03] |= 0x80;
	regs[0x11] &= ~0x80;

	/* write CRTC regs */
	for ( i = 0; i < VGA_NUM_CRTC_REGS; i++ ) {
		outb ( VGA_CRTC_INDEX, i );
		outb ( VGA_CRTC_DATA, *regs );
		regs++;
	}

	/* write GRAPHICS CONTROLLER regs */
	for ( i = 0; i < VGA_NUM_GC_REGS; i++ ) {
		outb ( VGA_GC_INDEX, i );
		outb ( VGA_GC_DATA, *regs );
		regs++;
	}

	/* write ATTRIBUTE CONTROLLER regs */
	for ( i = 0; i < VGA_NUM_AC_REGS; i++ ) {
		( void ) inb ( VGA_INSTAT_READ );
		outb ( VGA_AC_INDEX, i );
		outb ( VGA_AC_WRITE, *regs );
		regs++;
	}


	/* lock 16-color palette and unblank display */
	( void ) inb ( VGA_INSTAT_READ );
	outb ( VGA_AC_INDEX, 0x20 );

}

/**
 * Clears the VGA screen
 */
void VGA_clear_screen() {
	unsigned int x = 0;
	unsigned int y = 0;

	for ( y = 0; y < VGA_height; y++ ) {
		for ( x = 0; x < VGA_width; x++ ) {
			if ( isVESAon == ON ) { //if VESA is on, use 0xffffff as white since there is no pallete
				putPixel ( x, y, 0xffffff );
				//~ memset(vga_mem, 0xff, (widthVESA * heightVESA));

			} else if ( isVESAon == OFF ) { //if VESA is off, use 15 as white since there is a standard pallete
				putPixel ( x, y, 0 );
			}
		}
	}

}

void VGA_init ( int width, int height, int bpp ) {
	k_save();
	VGA_width = width;
	VGA_height = height;
	VGA_bpp = bpp;

	if ( VGA_width == 320 && VGA_height == 200 && VGA_bpp == 256 ) { //graphics mode 13
		write_registers ( mode_320_200_256 );
		putPixel = putPixel_simpleStd;
		VGA_clear_screen();
	} else if(width == 1024 && height == 768 && bpp == 24)
  {
    double_buffer = (u8int*)kmalloc((width * height) * (bpp / 8));

    //~ k_printf("\ndouble buffer: %h\n", double_buffer);
    
    memset(double_buffer, 0xff, width * height * (bpp / 8));

    //~ k_printf("double buffer content: %h %h %h %h %h %h %h\n", double_buffer[0], double_buffer[1], double_buffer[2], double_buffer[3], double_buffer[4], double_buffer[5]);
    //~ k_printf("Double buffer addresses: %h %h %h %h %h %h %h %h\n", &double_buffer[0], &double_buffer[1], &double_buffer[2], &double_buffer[3], &double_buffer[4], &double_buffer[5], &double_buffer[6]);

    //~ while(1);

    isVESAon = ON;
    setVesa(0x118); //1024x768x24
    putPixel = putPixel_VESA;
  
  }

	VGA = 1;
	//putRect(10,10,100,100,2);
	//putRect(20,20,80,80,3);

	//putGraphicString("abcdefghijklmnopqrstuvwxyz {|}~asdfghjkl", 0, 0, 3, 8);
	//putGraphicString("abcdefghijklmnop", 0, 0, 2, 8);
	//putGraphicString("qrstuvwxyzABCDEF", 0, 8, 2, 8);
	//putGraphicString("GHIJKLMNOPQRSTUV", 0, 16, 2, 8);
	//putGraphicString("WXYZ1234567890", 0, 24, 2, 8);


}



void putGraphicChar ( char *letter, int x, int y, int color, int fontSize ) {
	int offset, index, encodedLine, modNumber;

	int asciiCharToPrint = * ( letter ) /* - 65*/; //gest the index of the char to print

	int tmpX = x, tmpY = y;

	for ( y = tmpY; y < tmpY + fontSize; y++ ) { //clear the space for char
		for ( x = tmpX; x < tmpX + fontSize; x++ ) {
			putPixel ( x, y, 0 );
		}
	}

	for ( y = tmpY; y < tmpY + fontSize; y++ ) {
		encodedLine = font_std[asciiCharToPrint][y - tmpY];

		if ( encodedLine != 0 ) { //if there is atleast one pixel on that line

			for ( x = tmpX; x < tmpX + fontSize; x++ ) {
				offset = x + VGA_width * y; //place to draw

				index = fontSize - ( x - tmpX ) - 1; //finds the index of the number, if x == tmpX, ie: wants to draw 0 pixel if the array is 0b01111111, then it goes to 1 in -> direction

				modNumber = math_pow ( 2, index ); //finds the max number that the encoded line for the next step is an optimization

				if ( encodedLine - modNumber >= 0 ) { //if the max number is greater than the encoded line, then there must be no pixel on the index so skip drawing
					putPixel ( x, y, color );
					encodedLine = encodedLine - modNumber;
				}

			}
		}

	}

}

void putGraphicString ( char *string, int x, int y, int color, int fontSize ) {
	int length = strlen ( string ), moveX = ( int ) ( fontSize * ( 9 / 8 ) );

	int a;

	for ( a = 0; a < length; a++ ) {
		putGraphicChar ( string, x, y, color, fontSize );

		x = x + moveX;
		string++;
	}
}

putString ( char *string, int color ) {
	if ( chars_pos == chars_wide ) {

	}
}
