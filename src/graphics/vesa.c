/*
 * vesa.c
 *
 * Copyright 2013 JS-OS <js@duck-squirell>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */

#include "../common.h"
#include "vesa.h"
#include "graphics.h"
#include "../task.h"

#define wVESA     1024
#define hVESA     768
#define dVESA     32

int widthVESA, heightVESA, depthVESA;

unsigned char *vga_mem; //pointer where we assign our vga address

/*Typedef for VESA mode information*/
typedef struct MODE_INFO {
	unsigned short ModeAttributes       __attribute__ ( ( packed ) );
	unsigned char  WinAAttributes;//       __attribute__ ((packed));
	unsigned char  WinBAttributes;//       __attribute__ ((packed));
	unsigned short WinGranularity       __attribute__ ( ( packed ) );
	unsigned short WinSize              __attribute__ ( ( packed ) );
	unsigned short WinASegment          __attribute__ ( ( packed ) );
	unsigned short WinBSegment          __attribute__ ( ( packed ) );
	unsigned long  WinFuncPtr           __attribute__ ( ( packed ) );
	unsigned short BytesPerScanLine     __attribute__ ( ( packed ) );
	unsigned short XResolution          __attribute__ ( ( packed ) );
	unsigned short YResolution          __attribute__ ( ( packed ) );
	unsigned char  XCharSize;//            __attribute__ ((packed));
	unsigned char  YCharSize;//            __attribute__ ((packed));
	unsigned char  NumberOfPlanes;//       __attribute__ ((packed));
	unsigned char  BitsPerPixel;//         __attribute__ ((packed));
	unsigned char  NumberOfBanks;//        __attribute__ ((packed));
	unsigned char  MemoryModel;//          __attribute__ ((packed));
	unsigned char  BankSize;//             __attribute__ ((packed));
	unsigned char  NumberOfImagePages;//   __attribute__ ((packed));
	unsigned char  Reserved_page;//        __attribute__ ((packed));
	unsigned char  RedMaskSize;//          __attribute__ ((packed));
	unsigned char  RedMaskPos;//           __attribute__ ((packed));
	unsigned char  GreenMaskSize;//        __attribute__ ((packed));
	unsigned char  GreenMaskPos;//         __attribute__ ((packed));
	unsigned char  BlueMaskSize;//         __attribute__ ((packed));
	unsigned char  BlueMaskPos;//          __attribute__ ((packed));
	unsigned char  ReservedMaskSize;//     __attribute__ ((packed));
	unsigned char  ReservedMaskPos;//      __attribute__ ((packed));
	unsigned char  DirectColorModeInfo;//  __attribute__ ((packed));
	unsigned long  PhysBasePtr          __attribute__ ( ( packed ) );
	unsigned long  OffScreenMemOffset   __attribute__ ( ( packed ) );
	unsigned short OffScreenMemSize     __attribute__ ( ( packed ) );
	unsigned char  Reserved[206];//        __attribute__ ((packed));
} MODE_INFO;

/*Typedef for VESA information*/
typedef struct VESA_INFO {
	unsigned char  VESASignature[4];//     __attribute__ ((packed));
	unsigned short VESAVersion          __attribute__ ( ( packed ) );
	unsigned long  OEMStringPtr         __attribute__ ( ( packed ) );
	unsigned char  Capabilities[4];//      __attribute__ ((packed));
	unsigned long  VideoModePtr         __attribute__ ( ( packed ) );
	unsigned short TotalMemory          __attribute__ ( ( packed ) );
	unsigned short OemSoftwareRev       __attribute__ ( ( packed ) );
	unsigned long  OemVendorNamePtr     __attribute__ ( ( packed ) );
	unsigned long  OemProductNamePtr    __attribute__ ( ( packed ) );
	unsigned long  OemProductRevPtr     __attribute__ ( ( packed ) );
	unsigned char  Reserved[222];//        __attribute__ ((packed));
	unsigned char  OemData[256];//         __attribute__ ((packed));
} VESA_INFO;


extern window_t desktop;

/*Sets the bank if the Linear Frame Buffer is not supported or enabled*/
void setBank ( int bankNo ) {
	regs16_t regs;

	regs.ax = 0x4f05;
	regs.bx = 0x0;
	regs.dx = bankNo;

	int32 ( 0x10, &regs );

}

//sets up VESA for mode
void setVesa ( u32int mode ) {
	//stop any task switching, but do not stop interupts
	//TODO current_task->ready_to_run = FALSE;

	VESA_INFO info; //VESA information
	MODE_INFO vbeModeInfo; //VESA mode information

	regs16_t regs;

	/**Gets VESA information**/

	u32int buffer = ( u32int ) kmalloc ( sizeof ( VESA_INFO ) ) & 0xFFFFF; //sets the address for the buffer

	memcpy ( buffer, "VBE2", 4 );
	memset ( &regs, 0, sizeof ( regs ) ); //clears the registers typedef struct

	regs.ax = 0x4f00; //mode that gets VESA information
	regs.di = buffer & 0xF;
	regs.es = ( buffer>>4 ) & 0xFFFF;
	int32 ( 0x10, &regs ); //calls v86 interupt
	memcpy ( &info, buffer, sizeof ( VESA_INFO ) ); //copies info from the buffer to the info typedef struct

	//print VESA information
	//~ k_printf("\n\nVesa Signature: %s\n", info.VESASignature);
	//~ k_printf("\n\nVesa Version: %h\n", info.VESAVersion);
	//~ k_printf("\n\nVesa Video Modes: %h\n", info.VideoModePtr);

	/**Gests VESA mode information**/

	//allocates memory for the buffer that stores the MODE_INFO for the VESA mode
	u32int modeBuffer = ( u32int ) kmalloc ( sizeof ( MODE_INFO ) ) & 0xFFFFF;

	memset ( &regs, 0, sizeof ( regs ) ); //clears the registers typedef struct

	regs.ax = 0x4f01; //mode the gets the VESA mode information
	regs.di = modeBuffer & 0xF;
	regs.es = ( modeBuffer>>4 ) & 0xFFFF;
	regs.cx = mode; //mode to get the information for
	int32 ( 0x10, &regs );
	memcpy ( &vbeModeInfo, modeBuffer, sizeof ( MODE_INFO ) );

	widthVESA = vbeModeInfo.XResolution;
	heightVESA = vbeModeInfo.YResolution;
	depthVESA = vbeModeInfo.BitsPerPixel;

	//print VESA mode information
	//~ k_printf("\nBase Pointer: %h\n", (u32int)vbeModeInfo.PhysBasePtr);
	//~ k_printf("\nXRes: %d\n", (u32int)vbeModeInfo.XResolution);
	//~ k_printf("\nYRes: %d\n", (u32int)vbeModeInfo.YResolution);
	//~ k_printf("\nBits per pixel: %d\n", (u32int)vbeModeInfo.BitsPerPixel);
	//~ k_printf("\nExits status: %h\n", (u32int)regs.ax);

	/*Sets the Linear Frame Buffer address tp vga_mem and lfb variables*/
	vga_mem = ( u8int* ) vbeModeInfo.PhysBasePtr;
	u32int lfb = ( u32int ) vbeModeInfo.PhysBasePtr;

	/**Sets up the VESA mode**/
	regs.ax = 0x4f02; //mode the sets up VESA graphics

	/*sets up mode with a linear frame buffer, the logical or (| 0x4000) tells
	 * VESA VBE that we want to use a linear frame buffer*/
	regs.bx = ( mode | 0x4000 );
	int32 ( 0x10, &regs );

	//reenable the interupts and the tasks to run
	asm volatile ( "sti" );
	//TODO current_task->ready_to_run = TRUE;

}


/****DRAWING FUNCTIONS****/

#define MAX_WINDOWS 256

volatile window_t current_window;
volatile window_t *window_list;
