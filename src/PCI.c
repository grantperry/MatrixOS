#include <PCI.h>
#include <common.h>

void init_PCI() {
	serialf("Init PCI\n");
	outl(CONFIG_ADDRESS, pciGetConfAddr(0, 1, 1, 0));
	u32int in = inl(CONFIG_DATA);
	serialf("back: %h\n", in);
}

u32int pciGetConfAddr (u8int bus, u8int slot, u8int func, u8int offset) {
	u32int address = (u32int)((bus << 16) | (slot << 11) | (func << 8) | (offset & 0xfc) | ((u32int)0x80000000));
}
