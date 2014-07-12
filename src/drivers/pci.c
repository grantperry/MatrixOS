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

void init_PCI() {
	serialf("[PCI] Starting\n%d", sizeof(PCI_DEVICE_t));
	
	if(pci_find_bios()) {
		serialf("No PCI BIOS\n");
	}
	
	
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
						serialf("[PCI] VendorID: %d DeviceID: %d\n", tmp->head.VendorID, tmp->head.DeviceID);
	                }
	                else{ 
	                	serialf("[PCI] VendorID: %d DeviceID: %d\n", tmp->head.VendorID, tmp->head.DeviceID);
	                }
				}
			}
		}
	
	
	

#ifdef PCIE_H
	init_PCIe();
#endif//PCIE_H
}
