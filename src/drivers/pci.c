/*
//	PCI Express Driver
//	MatrixOS
*/
#include "pci.h"
#include "pci_list.h"
#include "pcie.h"
#include "../common.h"

PCI_BIOS_t *pci_bios;
PCI_DEVICE_t* ret;

static int pci_find_bios()
{
  unsigned long addr;
  unsigned char crc;
  int i;

  for (addr = 0xE0000; addr < 0xFFFFF; addr += 0x10)
    {
      pci_bios = (PCI_BIOS_t *) addr;
      if (pci_bios->Magic == 0x5F32335F)
	{
	  for (i = 0, crc = 0; i < (pci_bios->Pages * 16); i++)
	    crc += *((unsigned char *) (addr + i));
	  if (crc == 0)
	    {
          //found bios
          serialf("[PCI][FIND_BIOS] PCI BIOS found at 0x%h\n",addr);
	      return 0;
	    }
	}
    }
  //no bios
  serialf("[PCI][FIND_BIOS] PCI BIOS not found\n");
  return 1;
}

static u32int pci_read(int bus, int dev, int func, int reg)
{
  outl(0xCF8, ((unsigned long) 0x80000000 | (bus << 16) | (dev << 11) | (func << 8) | reg));
  return inl(0xCFC);
}

static PCI_DEVICE_t* pci_read_device(int bus, int dev, int func) {
	int place, total = sizeof(PCI_DEVICE_t) / sizeof(unsigned long);

	for (place = 0; place < total; place++) {
		((u32int*)ret)[place] =  pci_read(bus, dev, func, (place * sizeof (unsigned long)));
	}
	return ret;
}

static PCI_BRIDGE_t* pci_read_bridge(int bus, int dev, int func) {
	int place, total = sizeof(PCI_BRIDGE_t) / sizeof(unsigned long);

	for (place = 0; place < total; place++) {
		((u32int*)ret)[place] =  pci_read(bus, dev, func, (place * sizeof (unsigned long)));
	}
	return (PCI_BRIDGE_t*)ret;
}

u16int getVendorID(pci8 bus, pci8 device, pci8 function) {
	PCI_DEVICE_t *dev = pci_read_device(bus, device, function);
	return dev->head.VendorID;
}

pci8 getHeaderType(pci8 bus, pci8 device, pci8 function) {
	PCI_DEVICE_t *dev = pci_read_device(bus, device, function);
	return dev->head.HeaderType;
}

u16int getDeviceID(pci8 bus, pci8 device, pci8 function) {
	PCI_DEVICE_t *dev = pci_read_device(bus, device, function);
	return dev->head.DeviceID;
}

pci8 getBaseClass(pci8 bus, pci8 device, pci8 function) { //TODO Check what header type
	PCI_DEVICE_t *dev = pci_read_device(bus, device, function);
	return dev->head.ClassCode;
}

pci8 getSubClass(pci8 bus, pci8 device, pci8 function) {//TODO Check what header type
	PCI_DEVICE_t *dev = pci_read_device(bus, device, function);
	return dev->head.SubClass;
}

void checkFunction(u8int bus, u8int device, u8int function);

void checkDevice(u8int bus, u8int device) {
     pci8 function = 0;
     pci16 vendorID;
     pci16 deviceID;
     
     pci8 headerType;
 
     vendorID = getVendorID(bus, device, function);
     if(vendorID = 0xFFFF) {
     	serialf("[PCI] No Device\n");
     	return;
	}        // Device doesn't exist
     checkFunction(bus, device, function);
     headerType = getHeaderType(bus, device, function);
     if( (headerType & 0x80) != 0) {
         /* It is a multi-function device, so check remaining functions */
         serialf("[PCI] Multifunction device\n");
         for(function = 1; function < 8; function++) {
             if(getVendorID(bus, device, function) != 0xFFFF) {
                 checkFunction(bus, device, function);
             }
         }
     } else {
     	vendorID = getVendorID(bus, device, function);
     	deviceID = getDeviceID(bus, device, function);
     	serialf("[PCI] BUS:%d DEV:%d FUNC:%d VENDID:%d DEVID:%d\n", bus, device, function, vendorID, deviceID);
     }
 }

 void checkBus(u8int bus) {
     u8int device;
 
     for(device = 0; device < 32; device++) {
     	printf("[PCI] dev:%d\n", device);
     	sleep(1);
         checkDevice(bus, device);
     }
 }
 
 pci8 getSecondaryBus (pci8 bus, pci8 device, pci8 function) {
 	PCI_BRIDGE_t *dev = pci_read_bridge(bus, device, function);
 	return dev->SecondaryBusNumber;
 }
 
 void checkFunction(u8int bus, u8int device, u8int function) {
     u8int baseClass;
     u8int subClass;
     u8int secondaryBus;
 
     baseClass = getBaseClass(bus, device, function);
     subClass = getSubClass(bus, device, function);
     
     if( (baseClass == 0x06) && (subClass == 0x04) ) {
     	serialf("[PCI] Secondary Bus found: Bus:%d Device:%d Function:%d\n", bus, device, function);
         secondaryBus = getSecondaryBus(bus, device, function);
         checkBus(secondaryBus);
     }
 }
 
 void checkAllBuses(void) {
     pci8 function;
     pci8 bus;
     pci8 headerType;
	serialf("[PCI] Checking all busses\n");
     headerType = getHeaderType(0, 0, 0);
     if( (headerType & 0x80) == 0) {
         /* Single PCI host controller */
			serialf("[PCI] Single PCI Host Controller found\n");
			sleep(1);
         checkBus(0);
     } else {
         /* Multiple PCI host controllers */
         serialf("[PCI] Multiple PCI Host Controllers found\n");
         for(function = 0; function < 8; function++) {
             if(getVendorID(0, 0, function) != 0xFFFF) break;
             bus = function;
             checkBus(bus);
         }
     }
 }

void init_PCI() {
	serialf("[PCI] Starting\n%d", sizeof(PCI_DEVICE_t));
	
	if(pci_find_bios()) {
		serialf("No PCI BIOS\n");
	}
	
	
	checkAllBuses();
	return;
	
	ret = (PCI_DEVICE_t*)kmalloc(sizeof(PCI_DEVICE_t));
     
     //Now enumerate devices
     //if a driver is available, load it.
     //For now (boot) we will only load required drivers
     
     #ifdef DEBUG
     int invalid;
     #endif

     int bus, device, function;
     static int count = 0;
     PCI_DEVICE_t *tmp;
     
     for (bus = 0; bus <= 0xFF; bus++)
     {
          for (device = 0; device < 32; device++)
	      {
               for (function = 0; function < 8; function++)
               {
	                tmp = pci_read_device(bus, device, function);
	                if((tmp->head.VendorID == 0) || (tmp->head.VendorID == 0xFFFF) || (tmp->head.DeviceID == 0xFFFF)) {
						//serialf("[PCI] device NULL\n");
						//serialf("[PCI] %d %d %d VendorID: %d DeviceID: %d\n", bus, device, function, tmp->head.VendorID, tmp->head.DeviceID);
	                }
	                else{ 
	                	serialf("[PCI] %d %d %d VendorID: %d DeviceID: %d\n", bus, device, function, tmp->head.VendorID, tmp->head.DeviceID);
	                }
				}
			}
		}
	
	
	

#ifdef PCIE_H
	init_PCIe();
#endif//PCIE_H
}
