#include <PCI.h>
#include <common.h>

PCIDevice * pciDeviceTable;

void checkBus(u8int bus);

u8int pciConfReadByte(u8int bus, u8int slot, u8int func, u8int offset) {
	outl(CONFIG_ADDRESS, pciGetConfAddr(bus, slot, func, offset));
	return (u8int)((inl (CONFIG_DATA) >> ((offset & 2) * 8)) & 0xffff);
}

u16int pciConfReadWord(u8int bus, u8int slot, u8int func, u8int offset) {
	outl(CONFIG_ADDRESS, pciGetConfAddr(bus, slot, func, offset));
	return (u16int)((inl (CONFIG_DATA) >> ((offset & 2) * 8)) & 0xffff);
}

u32int pciConfReadLong(u8int bus, u8int slot, u8int func, u8int offset) {
	outl(CONFIG_ADDRESS, pciGetConfAddr(bus, slot, func, offset));
	return inl (CONFIG_DATA);
}

u16int getVendorID(u8int bus, u8int device, u8int function) {
	return pciConfReadWord(bus, device, function, VENDOR_ID);
}

u16int getDeviceID(u8int bus, u8int device, u8int function) {
	return pciConfReadWord(bus, device, function, DEVICE_ID);
}

u8int getBaseClass(u8int bus, u8int device, u8int function) {
	return pciConfReadByte(bus, device, function, CLASS_CODE);
}

u8int getSubClass(u8int bus, u8int device, u8int function) {
	return pciConfReadByte(bus, device, function, SUB_CLASS_CODE);
}

/*
//	return a pci device
//	class		= pci class code
//	subclass	= pci sub-class code
//	number		= if there are more than one the return the specifyide.
*/
PCIDevice * getPCIDevice(u8int class, u8int subclass, u8int number) {
	
	
}

u8int getHeaderType(u8int bus, u8int device, u8int function) {
	u8int type = (u8int)pciConfReadWord(bus, device, function, HEADER_TYPE);
	return type;
}

void pciVendorPrint(u8int bus, u8int slot, u8int func) {
	u16int vend = pciConfReadWord(bus, slot, func, VENDOR_ID);
	if (vend == 0x8086) {serialf("Intel Corp");}
	if (vend == 0x8087) {serialf("Intel");}
}

u8int getSecondaryBus(u8int bus, u8int device, u8int function) {
	serialf("TODO Get Secondary Bus.\n");
}

 void checkFunction(u8int bus, u8int device, u8int function) {
     u8int baseClass;
     u8int subClass;
     u8int secondaryBus;
     baseClass = getBaseClass(bus, device, function);
     subClass = getSubClass(bus, device, function);
     if( (baseClass == 0x06) && (subClass == 0x04) ) {
         secondaryBus = getSecondaryBus(bus, device, function);
         checkBus(secondaryBus);
     }
 }

void checkDevice(u8int bus, u8int device) {
     u8int function = 0;
 
     u16int vendorID = getVendorID(bus, device, function);
     if(vendorID = 0xFFFF) return;        // Device doesn't exist
     checkFunction(bus, device, function);
     u8int headerType = getHeaderType(bus, device, function);
     if( (headerType & 0x80) != 0) {
         /* It is a multi-function device, so check remaining functions */
         for(function = 1; function < 8; function++) {
             if(getVendorID(bus, device, function) != 0xFFFF) {
                 checkFunction(bus, device, function);
             }
         }
     }
 }

void checkBus(u8int bus) {
     u8int device;
 
     for(device = 0; device < 32; device++) {
         checkDevice(bus, device);
     }
 }


void init_PCI() {
	serialf("Init PCI\n");
	PCIDevice *point;
	pciDeviceTable = (PCIDevice*)kmalloc(sizeof(PCIDevice));
	point = pciDeviceTable;
	u8int bus, slot, function;
	
	checkBus(0); //check the first bus
	
}



u32int pciGetConfAddr (u8int bus, u8int slot, u8int func, u8int offset) {
	u32int address = (u32int)((bus << 16) | (slot << 11) | (func << 8) | (offset & 0xfc) | ((u32int)0x80000000));
}
