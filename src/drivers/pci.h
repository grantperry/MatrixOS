#ifndef PCI_H
#define PCI_H

#ifndef PCIE_H
	#include "pcie.h"
#endif//PCIE_H

// I/O Ports (32 bit)
#define PCI_CONFIG 0xCF8
#define PCI_DATA 0xCFC

// HEADER LENGTHS
#define PCI_CONFIGURATION_HEADER_LENGHT		256

// Initialise PCI
void init_PCI();
#endif//PCI_H
