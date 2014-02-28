#include "../common.h"

#ifndef GRAPHICS_H
#define GRAPHICS_H

void VGA_init(int width, int height, int bpp);

void (*putPixel)(int x, int y, int color);

void putRect(int x, int y, int width, int height, int fill);

void putGraphicChar(char *letter, int x, int y, int color, int fontSize);

void putGraphicString(char *string, int x, int y, int color, int fontSize);

#endif //GRAPHICS_H
