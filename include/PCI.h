#ifndef PCI_H
#define PCI_H
#include <common.h>
void init_PCI();

#define	CONFIG_ADDRESS	0xCF8
#define	CONFIG_DATA		0xCFC

#define	VENDOR_ID		0x00
#define	DEVICE_ID		0x02
#define	COMMAND			0x04
#define	STATUS			0x06
#define HEADER_TYPE		0x0E
#define CLASS_CODE		0x0B
#define SUB_CLASS_CODE	0x0A

#define BAR0			0x10
#define BAR1			0x14
#define BAR2			0x18
#define BAR3			0x1C
#define BAR4			0x20
#define BAR5			0x14

typedef struct PCIDevice{
	u8int bus, slot, function;
	u16int DeviceID;
	u16int VendorID;
	u8int ClassID;
	u8int SubclassID;
	
	struct PCIDevice *next;
} PCIDevice;



u32int pciGetConfAddr (u8int bus, u8int slot, u8int func, u8int offset);

#endif//PCI_H
