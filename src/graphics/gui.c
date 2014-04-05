#include "gui.h"
#include "../task.h"
#include "graphics.h"

int currentlook = 0;
int guirunning = 0;

extern unsigned char* vram;

unsigned char* gram = ( unsigned char* ) 0x00000;

void draw() {
	int i = 0;
	while (i <= VGA_width * VGA_height) {
		vram[i] = gram[i];
		i++;
	}
}

void putPixelg ( int x, int y, int color ) {
	int offset = x + VGA_width * y;
	gram[offset] = color;
}

void putRectg ( int x, int y, int width, int height, int fill ) {
	int offset, tmpX = x, tmpY = y;

	for ( y = tmpY; y < height + tmpY; y++ )
		for ( x = tmpX; x < width + tmpX; x++ ) {
			putPixelg ( x, y, fill );
		}
}

void putLineg(int x1, int y1, int x2, int y2, int fill)
{
  u32int slopeNum = math_abs(y2 - y1);
  u32int slopeDenom = math_abs(x2 - x1);

  int tmpX, tmpY;

  if(x1 > x2)
  {
    tmpX = x1;
    tmpY = y1;
  }else{
    tmpX = x2;
    tmpY = y2;
  }

  int originalX = tmpX;
  float slope = slopeNum / slopeDenom;
  float runningUp;

  int x, y, offset;
  for(y = 0; y < slopeNum + 1; y++)
  {
    for(x = 0; x < slopeDenom + 1; x++)
    {
      offset = tmpX + 320 * tmpY;
  
      gram[offset] = fill;
      tmpX++;
      
    }
    tmpX = originalX;
    tmpY++;
  }
  
}

void putHVLineg(int x, int y, int length, int color, char *HorV)
{
  int a, offset;

  if(*(HorV) == 'H') //draws a horizontal line
  {
    for(a = 0; a < length; a++)
    {
      offset = x + VGA_width * y;

      gram[offset] = color;

      x++;
    }
    
  }else if(*(HorV) == 'V') //draws a verticle line
  {
    for(a = 0; a < length; a++)
    {
      offset = x + VGA_width * y;

      gram[offset] = color;

      y++;
    }

  }
  
}

void guiv1() {
	while (guirunning) {
		drawBackground();
		drawBar();
		draw();
	}
	exit();
}


void startgui( int look ) {
	currentlook = look;
	guirunning = 1;
	if (look == 1) {
		start_task ( 200, 10, guiv1, 0, "guiv1" );
	}
}

void drawBackground() {
	if(currentlook == 1) {
		putRectg ( 0, 0, 320, 200, 1 );
	}
}
void drawBar() {
	if(currentlook == 1) {
		putRectg ( 0, VGA_height - 10, VGA_width, VGA_height, 2 );
		putHVLineg (0, VGA_height - 10, VGA_width, 4, "H");
	}
}
