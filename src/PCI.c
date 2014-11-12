#include <PCI.h>
#include <common.h>

u16int pciConfReadWord(u8int bus, u8int slot, u8int func, u8int offset) {
	outl(CONFIG_ADDRESS, pciGetConfAddr(bus, slot, func, offset));
	return (u16int)((inl (CONFIG_DATA) >> ((offset & 2) * 8)) & 0xffff);
}

u32int pciConfReadLong(u8int bus, u8int slot, u8int func, u8int offset) {
	outl(CONFIG_ADDRESS, pciGetConfAddr(bus, slot, func, offset));
	return inl (CONFIG_DATA);
}

void pciVendorPrint(u8int bus, u8int slot, u8int func) {
	u16int vend = pciConfReadWord(bus, slot, func, VENDOR_ID);
	if (vend == 0x8086) {serialf("Intel Corp");}
	if (vend == 0x8087) {serialf("Intel");}
}

void init_PCI() {
	serialf("Init PCI\n");
	
}



u32int pciGetConfAddr (u8int bus, u8int slot, u8int func, u8int offset) {
	u32int address = (u32int)((bus << 16) | (slot << 11) | (func << 8) | (offset & 0xfc) | ((u32int)0x80000000));
}
