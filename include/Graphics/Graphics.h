#ifndef GRAPHICS_H
#define GRAPHICS_H

u32int DispX;
u32int DispY;
u32int DispD;
u32int VESAMemLoc;

void (*drawPixel)(u32int X, u32int Y, u32int colour);
void drawRect(u32int X, u32int Y, u32int Xlen, u32int Ylen, u32int RBG);
void setDisplay(u32int mode);

#endif//GRAPHICS_H
