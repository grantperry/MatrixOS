#include <drivers/AHCI.h>
#include <drivers/pci.h>

s8int init_AHCI() {
	serialf("[AHCI] Init\n|-------> Finding on PCI\n");
	printPciAll();
}