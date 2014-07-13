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

typedef struct pci_bridge_header { //1
	struct pci_common_header head;
	pci32	BAR0;
	pci32	BAR1;
	pci8	PrimaryBusNumber;
	pci8	SecondaryBusNumber;
	pci8	SubordinateBusNumber;
	pci8	SecondaryLatencyTimer;
	pci8	IOBase;
	pci8	IOLimit;
	pci16	SecondaryStatus;
	pci16	MemoryBase;
	pci16	MemoryLimit;
	pci16	PrefetchableMemoryBase;
	pci16	PrefetchableMemoryLimit;
	pci32	PrefetchableBaseUpper32Bits;
	pci32	PrefetchableLimitUpper32Bits;
	pci16	IOBaseUpper16Bits;
	pci16	IOLimitUpper16Bits;
	pci8	CapabilityPointer;
	pci8	RES1;
	pci16	RES2;
	pci32	ExpansionROMAddress;
	pci8	InterruptLine;
	pci8	InteruptPin;
	pci16	BridgeControl;
} PCI_BRIDGE_t;

typedef struct pci_device_header { //0
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
