#include "../common.h"
#include "vesa.h"

//int32 runs a bios interrupt, located at v86.s
extern void int32(u8int intnum, regs16_t *regs);

unsigned char *vga_mem; //pointer where we assign our vga address

int widthVESA, heightVESA, depthVESA;

//sets up VESA for mode
void setVesa(u32int mode)
{
syscall_monitor_write("Hello VESA\n");
  //stop any task switching, but do not stop interupts
  current_task->ready_to_run = FALSE;

  VESA_INFO info; //VESA information
  MODE_INFO vbeModeInfo; //VESA mode information

  regs16_t regs;

  /**Gets VESA information**/

  u32int buffer = (u32int)kmalloc(sizeof(VESA_INFO)) & 0xFFFFF; //sets the address for the buffer

  memcpy(buffer, "VBE2", 4);
  memset(&regs, 0, sizeof(regs)); //clears the registers typedef struct

  regs.ax = 0x4f00; //mode that gets VESA information
  regs.di = buffer & 0xF;
  regs.es = (buffer>>4) & 0xFFFF;
  int32(0x10, &regs); //calls v86 interupt
  memcpy(&info, buffer, sizeof(VESA_INFO)); //copies info from the buffer to the info typedef struct

  //print VESA information
  //~ k_printf("\n\nVesa Signature: %s\n", info.VESASignature);
  //~ k_printf("\n\nVesa Version: %h\n", info.VESAVersion);
  //~ k_printf("\n\nVesa Video Modes: %h\n", info.VideoModePtr);

  /**Gests VESA mode information**/

  //allocates memory for the buffer that stores the MODE_INFO for the VESA mode
  u32int modeBuffer = (u32int)kmalloc(sizeof(MODE_INFO)) & 0xFFFFF;

  memset(&regs, 0, sizeof(regs)); //clears the registers typedef struct

  regs.ax = 0x4f01; //mode the gets the VESA mode information
  regs.di = modeBuffer & 0xF;
  regs.es = (modeBuffer>>4) & 0xFFFF;
  regs.cx = mode; //mode to get the information for
  int32(0x10, &regs);
  memcpy(&vbeModeInfo, modeBuffer, sizeof(MODE_INFO));

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
  vga_mem = (u8int*)vbeModeInfo.PhysBasePtr;
  u32int lfb = (u32int)vbeModeInfo.PhysBasePtr;

  /**Sets up the VESA mode**/
  regs.ax = 0x4f02; //mode the sets up VESA graphics

  /*sets up mode with a linear frame buffer, the logical or (| 0x4000) tells
   * VESA VBE that we want to use a linear frame buffer*/
  regs.bx = (mode | 0x4000);
  int32(0x10, &regs);

  //reenable the interupts and the tasks to run
  asm volatile("sti");
  current_task->ready_to_run = TRUE;

}
