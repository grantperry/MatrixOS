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

#define BAR0			0x10
#define BAR1			0x14
#define BAR2			0x18
#define BAR3			0x1C
#define BAR4			0x20
#define BAR5			0x14



u32int pciGetConfAddr (u8int bus, u8int slot, u8int func, u8int offset);

#endif//PCI_H
