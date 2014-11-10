#ifndef PCI_H
#define PCI_H
#include <common.h>
void init_PCI();

#define	CONFIG_ADDRESS	0xCF8
#define	CONFIG_DATA		0xCFC

u32int pciGetConfAddr (u8int bus, u8int slot, u8int func, u8int offset);

#endif//PCI_H
