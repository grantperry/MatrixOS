// MatrixOS - AHCI.h
// header for AHCI.c 
// Contains information about specifications

#ifndef AHCI_H
#define AHCI_H

#include <stdio.h>

s8int init_AHCI();

typedef enum
{
	FIS_TYPE_REG_H2D	= 0x27,	// Register FIS - host to device
	FIS_TYPE_REG_D2H	= 0x34,	// Register FIS - device to host
	FIS_TYPE_DMA_ACT	= 0x39,	// DMA activate FIS - device to host
	FIS_TYPE_DMA_SETUP	= 0x41,	// DMA setup FIS - bidirectional
	FIS_TYPE_DATA		= 0x46,	// Data FIS - bidirectional
	FIS_TYPE_BIST		= 0x58,	// BIST activate FIS - bidirectional
	FIS_TYPE_PIO_SETUP	= 0x5F,	// PIO setup FIS - device to host
	FIS_TYPE_DEV_BITS	= 0xA1,	// Set device bits FIS - device to host
} FIS_TYPE;

/*
 ALL RESERVED REG'S NEED TO BE ZERO(0)
 */

typedef struct tagFIS_REG_H2D // A host to device register FIS is used by the host to send command or control to a device.
{
	// u32int 0
	u8int	fis_type;	// FIS_TYPE_REG_H2D
	
	u8int	pmport:4;	// Port multiplier
	u8int	rsv0:3;		// Reserved
	u8int	c:1;		// 1: Command, 0: Control
	
	u8int	command;	// Command register
	u8int	featurel;	// Feature register, 7:0
	
	// u32int 1
	u8int	lba0;		// LBA low register, 7:0
	u8int	lba1;		// LBA mid register, 15:8
	u8int	lba2;		// LBA high register, 23:16
	u8int	device;		// Device register
	
	// u32int 2
	u8int	lba3;		// LBA register, 31:24
	u8int	lba4;		// LBA register, 39:32
	u8int	lba5;		// LBA register, 47:40
	u8int	featureh;	// Feature register, 15:8
	
	// u32int 3
	u8int	countl;		// Count register, 7:0
	u8int	counth;		// Count register, 15:8
	u8int	icc;		// Isochronous command completion
	u8int	control;	// Control register
	
	// u32int 4
	u8int	rsv1[4];	// Reserved
} FIS_REG_H2D;

typedef struct tagFIS_REG_D2H // A device to host register FIS is used by the device to notify the host that some ATA register has changed. It contains the updated task files such as status, error and other registers.
{
	// u32int 0
	u8int	fis_type;    // FIS_TYPE_REG_D2H
	
	u8int	pmport:4;    // Port multiplier
	u8int	rsv0:2;      // Reserved
	u8int	i:1;         // Interrupt bit
	u8int	rsv1:1;      // Reserved
	
	u8int	status;      // Status register
	u8int	error;       // Error register
	
	// u32int 1
	u8int	lba0;        // LBA low register, 7:0
	u8int	lba1;        // LBA mid register, 15:8
	u8int	lba2;        // LBA high register, 23:16
	u8int	device;      // Device register
	
	// u32int 2
	u8int	lba3;        // LBA register, 31:24
	u8int	lba4;        // LBA register, 39:32
	u8int	lba5;        // LBA register, 47:40
	u8int	rsv2;        // Reserved
	
	// u32int 3
	u8int	countl;      // Count register, 7:0
	u8int	counth;      // Count register, 15:8
	u8int	rsv3[2];     // Reserved
	
	// u32int 4
	u8int	rsv4[4];     // Reserved
} FIS_REG_D2H;

typedef struct tagFIS_DATA // This FIS is used by the host or device to send data payload. The data size can be varied.
{
	// u32int 0
	u8int	fis_type;	// FIS_TYPE_DATA
	
	u8int	pmport:4;	// Port multiplier
	u8int	rsv0:4;		// Reserved
	
	u8int	rsv1[2];	// Reserved
	
	// u32int 1 ~ N
	u32int	data[1];	// Payload
} FIS_DATA;

typedef struct tagFIS_PIO_SETUP //This FIS is used by the device to tell the host that it’s about to send or ready to receive a PIO data payload.
{
	// u32int 0
	u8int	fis_type;	// FIS_TYPE_PIO_SETUP
	
	u8int	pmport:4;	// Port multiplier
	u8int	rsv0:1;		// Reserved
	u8int	d:1;		// Data transfer direction, 1 - device to host
	u8int	i:1;		// Interrupt bit
	u8int	rsv1:1;
	
	u8int	status;		// Status register
	u8int	error;		// Error register
	
	// u32int 1
	u8int	lba0;		// LBA low register, 7:0
	u8int	lba1;		// LBA mid register, 15:8
	u8int	lba2;		// LBA high register, 23:16
	u8int	device;		// Device register
	
	// u32int 2
	u8int	lba3;		// LBA register, 31:24
	u8int	lba4;		// LBA register, 39:32
	u8int	lba5;		// LBA register, 47:40
	u8int	rsv2;		// Reserved
	
	// u32int 3
	u8int	countl;		// Count register, 7:0
	u8int	counth;		// Count register, 15:8
	u8int	rsv3;		// Reserved
	u8int	e_status;	// New value of status register
	
	// u32int 4
	u16int	tc;		// Transfer count
	u8int	rsv4[2];	// Reserved
} FIS_PIO_SETUP;

typedef struct tagFIS_DMA_SETUP // Device to Host
{
	// u32int 0
	u8int	fis_type;	// FIS_TYPE_DMA_SETUP
	
	u8int	pmport:4;	// Port multiplier
	u8int	rsv0:1;		// Reserved
	u8int	d:1;		// Data transfer direction, 1 - device to host
	u8int	i:1;		// Interrupt bit
	u8int	a:1;            // Auto-activate. Specifies if DMA Activate FIS is needed
	
	u8int    rsved[2];       // Reserved
	
	//u32int 1&2
	
	u32int  DMAbufferID1;    // DMA Buffer Identifier. Used to Identify DMA buffer in host memory. SATA Spec says host specific and not in Spec. Trying AHCI spec might work.
	u32int  DMAbufferID2;    // DMA Buffer Identifier. Used to Identify DMA buffer in host memory. SATA Spec says host specific and not in Spec. Trying AHCI spec might work.
	
	//u32int 3
	u32int   rsvd;           //More reserved
	
	//u32int 4
	u32int   DMAbufOffset;   //u8int offset into buffer. First 2 bits must be 0
	
	//u32int 5
	u32int   TransferCount;  //Number of u8ints to transfer. Bit 0 must be 0
	
	//u32int 6
	u32int   resvd;          //Reserved
	
} FIS_DMA_SETUP;

typedef volatile struct tagHBA_PORT
{
	u32int	clb;		// 0x00, command list base address, 1K-u8int aligned
	u32int	clbu;		// 0x04, command list base address upper 32 bits
	u32int	fb;		// 0x08, FIS base address, 256-u8int aligned
	u32int	fbu;		// 0x0C, FIS base address upper 32 bits
	u32int	is;		// 0x10, interrupt status
	u32int	ie;		// 0x14, interrupt enable
	u32int	cmd;		// 0x18, command and status
	u32int	rsv0;		// 0x1C, Reserved
	u32int	tfd;		// 0x20, task file data
	u32int	sig;		// 0x24, signature
	u32int	ssts;		// 0x28, SATA status (SCR0:SStatus)
	u32int	sctl;		// 0x2C, SATA control (SCR2:SControl)
	u32int	serr;		// 0x30, SATA error (SCR1:SError)
	u32int	sact;		// 0x34, SATA active (SCR3:SActive)
	u32int	ci;		// 0x38, command issue
	u32int	sntf;		// 0x3C, SATA notification (SCR4:SNotification)
	u32int	fbs;		// 0x40, FIS-based switch control
	u32int	rsv1[11];	// 0x44 ~ 0x6F, Reserved
	u32int	vendor[4];	// 0x70 ~ 0x7F, vendor specific
} HBA_PORT;

typedef volatile struct tagHBA_MEM
{
	// 0x00 - 0x2B, Generic Host Control
	u32int	cap;		// 0x00, Host capability
	u32int	ghc;		// 0x04, Global host control
	u32int	is;		// 0x08, Interrupt status
	u32int	pi;		// 0x0C, Port implemented
	u32int	vs;		// 0x10, Version
	u32int	ccc_ctl;	// 0x14, Command completion coalescing control
	u32int	ccc_pts;	// 0x18, Command completion coalescing ports
	u32int	em_loc;		// 0x1C, Enclosure management location
	u32int	em_ctl;		// 0x20, Enclosure management control
	u32int	cap2;		// 0x24, Host capabilities extended
	u32int	bohc;		// 0x28, BIOS/OS handoff control and status
	
	// 0x2C - 0x9F, Reserved
	u8int	rsv[0xA0-0x2C];
	
	// 0xA0 - 0xFF, Vendor specific registers
	u8int	vendor[0x100-0xA0];
	
	// 0x100 - 0x10FF, Port control registers
	HBA_PORT	ports[1];	// 1 ~ 32
} HBA_MEM;

typedef volatile struct tagHBA_FIS
{
	// 0x00
	FIS_DMA_SETUP	dsfis;		// DMA Setup FIS
	u8int		pad0[4];
	
	// 0x20
	FIS_PIO_SETUP	psfis;		// PIO Setup FIS
	u8int		pad1[12];
	
	// 0x40
	FIS_REG_D2H	rfis;		// Register – Device to Host FIS
	u8int		pad2[4];
	
	// 0x58
	//FIS_DEV_BITS	sdbfis;		// Set Device Bit FIS//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// 0x60
	u8int		ufis[64];
	
	// 0xA0
	u8int		rsv[0x100-0xA0];
} HBA_FIS;


#endif//AHCI_H