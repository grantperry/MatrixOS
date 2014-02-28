#include "graphics.h"
#include "../common.h"
#include "font.h"
#include "../math/math.h"

//the vga identifiers
u32int VGA_width;
u32int VGA_height;
u32int VGA_bpp;

void (*putPixel)(int, int, int);
extern int font_std[26][8];

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
	//putRect(10,10,100,100,2);
	//putRect(20,20,80,80,3);
	putGraphicString("HELLO", 110, 100, 2, 8);
	putGraphicString("WORLD", 110 + (8*6), 100, 2, 8);

}

void putGraphicChar(char *letter, int x, int y, int color, int fontSize)
{
  int offset, index, encodedLine, modNumber;

  int asciiCharToPrint = *(letter) - 65; //gest the index of the char to print

  int tmpX = x, tmpY = y;

  for(y = tmpY; y < tmpY + fontSize; y++)
  {
    encodedLine = font_std[asciiCharToPrint][y - tmpY];

    if(encodedLine != 0) //if there is atleast one pixel on that line
    {
      
      for(x = tmpX; x < tmpX + fontSize; x++)
      {
        offset = x + VGA_width * y; //place to draw

        index = fontSize - (x - tmpX) - 1; //finds the index of the number, if x == tmpX, ie: wants to draw 0 pixel if the array is 0b01111111, then it goes to 1 in -> direction

        modNumber = math_pow(2, index); //finds the max number that the encoded line for the next step is an optimization

        if(encodedLine - modNumber >= 0) //if the max number is greater than the encoded line, then there must be no pixel on the index so skip drawing
        {
          putPixel(x, y, color);
          encodedLine = encodedLine - modNumber;
        }
        
      }
    }
      
  }
  
}

void putGraphicString(char *string, int x, int y, int color, int fontSize)
{
  int length = strlen(string), moveX = (int)(fontSize * (9 / 8));

  int a;
  for(a = 0; a < length; a++)
  {
    putGraphicChar(string, x, y, color, fontSize);

    x = x + moveX;
    string++;
  }
}
