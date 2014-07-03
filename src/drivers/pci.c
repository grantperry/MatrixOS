/*
//	PCI Express Driver
//	MatrixOS
//	Refer to ~MatrixOS/docs/references/phy-interface-pci-express-sata-usb31-architectures-ver43.pdf
*/
#include "pci.h"

void init_PCI() {
	
	#ifdef PCIE_H
		init_PCIe();
	#endif//PCIE_H
}
