#include <drivers/AHCI.h>
#include <drivers/pci.h>

s8int init_AHCI() {
	serialf("[AHCI] Init\n|-------> Finding on PCI\n");
	checkBus(0);
	u8int i = 0;
	u8int e = 0;
	u8int f = 0;
	for(i = 0; i <= 255; i++){
		for (e = 0; e <= 32; e++) {
			for (f = 0; f <= 32; f++) {
				if (getVendorID(i, e, f) != 0xFFFFFFFF) {
					serialf("\t|---------#%d.%d.%d#\n", i, e, f);
					serialf("\t|--------->venid:\t\t%h\n\t|--------->devid:\t\t%h\n", pciConfReadWord(i, e, 0, VENDOR_ID), pciConfReadWord(i, e, 0, DEVICE_ID));
					serialf("\t|--------->command:\t\t%h\n\t|--------->status:\t\t%h\n", pciConfReadWord(i, e, 0, COMMAND), pciConfReadWord(i, e, 0, STATUS));
					serialf("\t|--------->Class:\t\t%h\n\t|--------->SubClass:\t\t%h\n", pciConfReadByte(i, e, 0, CLASS_CODE), pciConfReadByte(i, e, 0, SUB_CLASS_CODE));
					serialf("\t|--------->HeadType:\t\t%h\n\t|--------->BIST:\t\t%h\n", pciConfReadByte(i, e, 0, HEADER_TYPE), pciConfReadByte(i, e, 0, 0x0F));
				}
			if (i == 255) {return;}
			}
		}
	}	
}