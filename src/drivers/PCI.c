#include <drivers/pci.h>
#include <common.h>

void checkBus(u8int bus);

u32int getPciConfAddress(u8int bus, u8int slot, u8int func, u8int offset) {
	u32int address;
    u32int lbus  = (u32int)bus;
    u32int lslot = (u32int)slot;
    u32int lfunc = (u32int)func;
	address = (u32int)((lbus << 16) | (lslot << 11) |
					   (lfunc << 8) | (offset & 0xfc) | ((u32int)0x80000000));
}

u8int pciConfReadByte(u8int bus, u8int slot, u8int func, u8int offset) {
	u32int address;
    u32int lbus  = (u32int)bus;
    u32int lslot = (u32int)slot;
    u32int lfunc = (u32int)func;
    u16int tmp = 0;
	
    /* create configuration address as per Figure 1 */
    address = (u32int)((lbus << 16) | (lslot << 11) |
						 (lfunc << 8) | (offset & 0xfc) | ((u32int)0x80000000));
	
	outl(CONFIG_ADDRESS, address);
	return (u8int)((inl (CONFIG_DATA) >> ((offset & 2) * 8)) & 0xffff);
}

u16int pciConfReadWord(u8int bus, u8int slot, u8int func, u8int offset) {
	u32int address;
    u32int lbus  = (u32int)bus;
    u32int lslot = (u32int)slot;
    u32int lfunc = (u32int)func;
    u16int tmp = 0;
	
    /* create configuration address as per Figure 1 */
    address = (u32int)((lbus << 16) | (lslot << 11) |
						 (lfunc << 8) | (offset & 0xfc) | ((u32int)0x80000000));
	
	//serialf("addr: %h\n", address);
	
	outl(CONFIG_ADDRESS, address);
	return (u16int)((inl (CONFIG_DATA) >> ((offset & 2) * 8)) & 0xffff);
}

u32int pciConfReadLong(u8int bus, u8int slot, u8int func, u8int offset) {
	u32int address;
    u32int lbus  = (u32int)bus;
    u32int lslot = (u32int)slot;
    u32int lfunc = (u32int)func;
    u16int tmp = 0;
	
    /* create configuration address as per Figure 1 */
    address = (u32int)((lbus << 16) | (lslot << 11) |
						 (lfunc << 8) | (offset & 0xfc) | ((u32int)0x80000000));
	
	outl(CONFIG_ADDRESS, address);
	return inl (CONFIG_DATA);
}

u16int getVendorID(u8int bus, u8int device, u8int function) {
	return (u16int)pciConfReadWord(bus, device, function, VENDOR_ID);
}

u16int getDeviceID(u8int bus, u8int device, u8int function) {
	return (u16int)pciConfReadWord(bus, device, function, DEVICE_ID);
}

u8int getBaseClass(u8int bus, u8int device, u8int function) {
	return (u8int)pciConfReadByte(bus, device, function, CLASS_CODE);
}

u8int getSubClass(u8int bus, u8int device, u8int function) {
	return (u8int)pciConfReadByte(bus, device, function, SUB_CLASS_CODE);
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
	 serialf("venid:%h\n", getVendorID(bus, device, function));
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

void printPciAll() {
	u8int i = 0;
	u8int e = 0;
	u8int f = 0;
	for(i = 0; i <= 255; i++){
		for (e = 0; e <= 32; e++) {
			//for (f = 0; f <= 32; f++) {
				if (getVendorID(i, e, f) != 0xFFFFFFFF) {
					serialf("\t|---------#%d.%d.%d#\n", i, e, f);
					serialf("\t|--------->venid:\t\t%h\n\t|--------->devid:\t\t%h\n", pciConfReadWord(i, e, 0, VENDOR_ID), pciConfReadWord(i, e, 0, DEVICE_ID));
					serialf("\t|--------->command:\t\t%h\n\t|--------->status:\t\t%h\n", pciConfReadWord(i, e, 0, COMMAND), pciConfReadWord(i, e, 0, STATUS));
					serialf("\t|--------->Class:\t\t%h\n\t|--------->SubClass:\t\t%h\n", pciConfReadByte(i, e, 0, CLASS_CODE), pciConfReadByte(i, e, 0, SUB_CLASS_CODE));
					serialf("\t|--------->HeadType:\t\t%h\n\t|--------->BIST:\t\t%h\n", pciConfReadByte(i, e, 0, HEADER_TYPE), pciConfReadByte(i, e, 0, 0x0F));
				}
				if (i == 255) {return;}
			//}
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
	u8int bus, slot, function;
	
	//checkBus(0); //check the first bus
	
}

