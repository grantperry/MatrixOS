#include <stdio.h>
#include <Graphics/BackBuffer.h>
#include <Graphics/Graphics.h>
#include <VESA/VESA.h>

u8int *BackBuffer = 0;
u32int bbSize = 0;

void init_BackBuffer(u32int X, u32int Y, u32int D) {
	bbSize = (X * Y) * D;
	serialf("Creating Graphcs Back Buffer\nSize: %dbytes\n", bbSize);
	BackBuffer = (u8int*)kmalloc(bbSize);
}
