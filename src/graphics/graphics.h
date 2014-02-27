#include "../common.h"

#ifndef GRAPHICS_H
#define GRAPHICS_H

void VGA_init(int width, int height, int bpp);

void (*putPixel)(int x, int y, int color);

void putRect(int x, int y, int width, int height, int fill);

#endif //GRAPHICS_H
