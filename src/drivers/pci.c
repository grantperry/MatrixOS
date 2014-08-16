/*
//	PCI Express Driver
//	MatrixOS
*/
#include "pci.h"
#include "pci_list.h"
#include "../common.h"



void pci_write_field(u32int device, int field, int size, u32int value) {
	outl(PCI_ADDRESS_PORT, pci_get_addr(device, field));
	outl(PCI_VALUE_PORT, value);
}

u32int pci_read_field(u32int device, int field, int size) {
	outl(PCI_ADDRESS_PORT, pci_get_addr(device, field));

	if (size == 4) {
		u32int t = inl(PCI_VALUE_PORT);
		return t;
	} else if (size == 2) {
		u16int t = ins(PCI_VALUE_PORT + (field & 2));
		return t;
	} else if (size == 1) {
		u8int t = inb(PCI_VALUE_PORT + (field & 3));
		return t;
	}
	return 0xFFFF;
}

u16int pci_find_type(u32int dev) {
	return (pci_read_field(dev, PCI_CLASS, 1) << 8) | pci_read_field(dev, PCI_SUBCLASS, 1);
}

const char * pci_vendor_lookup(unsigned short vendor_id) {
	unsigned int i = 0;
	for (; i < PCI_VENTABLE_LEN; ++i) {
		if (PciVenTable[i].VenId == vendor_id) {
			return PciVenTable[i].VenFull;
		}
	}
	return "";
}

const char * pci_device_lookup(unsigned short vendor_id, unsigned short device_id) {
	unsigned int i = 0;
	for (; i < PCI_DEVTABLE_LEN; ++i) {
		if (PciDevTable[i].VenId == vendor_id && PciDevTable[i].DevId == device_id) {
			return PciDevTable[i].ChipDesc;
		}
	}
	return "";
}

void pci_scan_hit(pci_func_t f, u32int dev, void * extra) {
	int dev_vend = (int)pci_read_field(dev, PCI_VENDOR_ID, 2);
	int dev_dvid = (int)pci_read_field(dev, PCI_DEVICE_ID, 2);

	f(dev, dev_vend, dev_dvid, extra);
}

void pci_scan_func(pci_func_t f, int type, int bus, int slot, int func, void * extra) {
	u32int dev = pci_box_device(bus, slot, func);
	if (type == -1 || type == pci_find_type(dev)) {
		pci_scan_hit(f, dev, extra);
	}
	if (pci_find_type(dev) == PCI_TYPE_BRIDGE) {
		pci_scan_bus(f, type, pci_read_field(dev, PCI_SECONDARY_BUS, 1), extra);
	}
}

void pci_scan_slot(pci_func_t f, int type, int bus, int slot, void * extra) {
	u32int dev = pci_box_device(bus, slot, 0);
	if (pci_read_field(dev, PCI_VENDOR_ID, 2) == PCI_NONE) {
		return;
	}
	pci_scan_func(f, type, bus, slot, 0, extra);
	if (!pci_read_field(dev, PCI_HEADER_TYPE, 1)) {
		return;
	}
	int func = 1;
	for (; func < 8; func++) {
		u32int dev = pci_box_device(bus, slot, func);
		if (pci_read_field(dev, PCI_VENDOR_ID, 2) != PCI_NONE) {
			pci_scan_func(f, type, bus, slot, func, extra);
		}
	}
}

void pci_scan_bus(pci_func_t f, int type, int bus, void * extra) {
	int slot = 0;
	for (; slot < 32; ++slot) {
		pci_scan_slot(f, type, bus, slot, extra);
	}
}



static void scan_list(u32int device, u32int vendorid, u32int deviceid, void * extra) {
	serialf("[PCI] Ven: %h  Dev: %h  STAT: %d  COMM: %d\n", vendorid, deviceid, pci_read_field(device, PCI_STATUS, 2), pci_read_field(device, PCI_COMMAND, 2));
}




void pci_scan(pci_func_t f, int type, void * extra) {
	pci_scan_bus(f, type, 0, extra);

	if (!pci_read_field(0, PCI_HEADER_TYPE, 1)) {
		return;
	}

	int func = 1;
	for (; func < 8; ++func) {
		u32int dev = pci_box_device(0, 0, func);
		if (pci_read_field(dev, PCI_VENDOR_ID, 2) != PCI_NONE) {
			pci_scan_bus(f, type, func, extra);
		} else {
			break;
		}
	}
}


void init_PCI() {
	pci_scan((pci_func_t)scan_list, -1, 0);

#ifdef PCIE_H
	init_PCIe();
#endif//PCIE_H
}
