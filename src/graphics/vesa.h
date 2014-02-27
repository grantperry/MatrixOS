
#ifndef VESA_H
#define VESA_H

	#define wVESA     1024
	#define hVESA     768
	#define dVESA     32

// define our structure
typedef struct __attribute__ ((packed)) {
  unsigned short di, si, bp, sp, bx, dx, cx, ax;
  unsigned short gs, fs, es, ds, eflags;
} regs16_t;

int widthVESA, heightVESA, depthVESA;

unsigned char *vga_mem; //pointer where we assign our vga address

/*Typedef for VESA mode information*/
typedef struct MODE_INFO
{
  unsigned short ModeAttributes       __attribute__ ((packed));
  unsigned char  WinAAttributes;//       __attribute__ ((packed));
  unsigned char  WinBAttributes;//       __attribute__ ((packed));
  unsigned short WinGranularity       __attribute__ ((packed));
  unsigned short WinSize              __attribute__ ((packed));
  unsigned short WinASegment          __attribute__ ((packed));
  unsigned short WinBSegment          __attribute__ ((packed));
  unsigned long  WinFuncPtr           __attribute__ ((packed));
  unsigned short BytesPerScanLine     __attribute__ ((packed));
  unsigned short XResolution          __attribute__ ((packed));
  unsigned short YResolution          __attribute__ ((packed));
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
  unsigned long  PhysBasePtr          __attribute__ ((packed));
  unsigned long  OffScreenMemOffset   __attribute__ ((packed));
  unsigned short OffScreenMemSize     __attribute__ ((packed));
  unsigned char  Reserved[206];//        __attribute__ ((packed));
} MODE_INFO;

/*Typedef for VESA information*/
typedef struct VESA_INFO
{
  unsigned char  VESASignature[4];//     __attribute__ ((packed));
  unsigned short VESAVersion          __attribute__ ((packed));
  unsigned long  OEMStringPtr         __attribute__ ((packed));
  unsigned char  Capabilities[4];//      __attribute__ ((packed));
  unsigned long  VideoModePtr         __attribute__ ((packed));
  unsigned short TotalMemory          __attribute__ ((packed));
  unsigned short OemSoftwareRev       __attribute__ ((packed));
  unsigned long  OemVendorNamePtr     __attribute__ ((packed));
  unsigned long  OemProductNamePtr    __attribute__ ((packed));
  unsigned long  OemProductRevPtr     __attribute__ ((packed));
  unsigned char  Reserved[222];//        __attribute__ ((packed));
  unsigned char  OemData[256];//         __attribute__ ((packed));
} VESA_INFO;

#endif //VESA_H
