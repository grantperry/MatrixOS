#ifndef PCI_H
#define PCI_H
#include "../common.h"
#ifndef PCIE_H
#include "pcie.h"
#endif//PCIE_H

typedef unsigned char pci8;
typedef unsigned short pci16;
typedef unsigned int pci32;

typedef struct {
	unsigned long Magic;
	unsigned long EntryPoint;
	unsigned char Version;
	unsigned char Pages;
	unsigned char CRC;
} PCI_BIOS_t;

typedef struct pci_common_header {
	pci16	VendorID;
	pci16	DeviceID;
	pci16	Command;
	pci16	Status;
	pci8	RevisionID;
	pci8	ProgIF;
	pci8	SubClass;
	pci8	ClassCode;
	pci8	CacheLineSize;
	pci8	MasterLatencyTimer;
	pci8 	HeaderType;
	pci8	BIST;
} pci_common_header_t;

typedef struct pci_device_header {
	struct pci_common_header head;
	pci32	BAR0;
	pci32	BAR1;
	pci32	BAR2;
	pci32	BAR3;
	pci32	BAR4;
	pci32	BAR5;
	pci32	CardbusCISPtr;
	pci16	SubVendorID;
	pci16	SubsystemID;
	pci32	ExRomAddress;
	pci8	CapabilitiesPtr;
	pci8	RES1;
	pci16	RES2;
	pci32	RES3;
	pci8	IntLine;
	pci8	IntPin;
	pci8	Min_Gnt;
	pci8	Max_Lat;
} PCI_DEVICE_t;

// Initialise PCI
void init_PCI();
#endif//PCI_H
