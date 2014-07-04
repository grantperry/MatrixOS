#ifndef PCI_H
#define PCI_H
#include "../common.h"
#ifndef PCIE_H
#include "pcie.h"
#endif//PCIE_H

#define PCI_VENDOR_ID            0x00 // 2
#define PCI_DEVICE_ID            0x02 // 2
#define PCI_COMMAND              0x04 // 2
#define PCI_STATUS               0x06 // 2
#define PCI_REVISION_ID          0x08 // 1

#define PCI_PROG_IF              0x09 // 1
#define PCI_SUBCLASS             0x0a // 1
#define PCI_CLASS                0x0b // 1
#define PCI_CACHE_LINE_SIZE      0x0c // 1
#define PCI_LATENCY_TIMER        0x0d // 1
#define PCI_HEADER_TYPE          0x0e // 1
#define PCI_BIST                 0x0f // 1
#define PCI_BAR0                 0x10 // 4
#define PCI_BAR1                 0x14 // 4
#define PCI_BAR2                 0x18 // 4
#define PCI_BAR3                 0x1C // 4
#define PCI_BAR4                 0x20 // 4
#define PCI_BAR5                 0x24 // 4

#define PCI_INTERRUPT_LINE       0x3C // 1

#define PCI_SECONDARY_BUS        0x09 // 1

#define PCI_HEADER_TYPE_DEVICE  0
#define PCI_HEADER_TYPE_BRIDGE  1
#define PCI_HEADER_TYPE_CARDBUS 2

#define PCI_TYPE_BRIDGE 0x0604
#define PCI_TYPE_SATA   0x0106

#define PCI_ADDRESS_PORT 0xCF8
#define PCI_VALUE_PORT   0xCFC

#define PCI_NONE 0xFFFF

typedef void (*pci_func_t)(u32int device, u16int vendor_id, u16int device_id, void * extra);

static inline int pci_extract_bus(u32int device) {
	return (u8int)((device >> 16));
}
static inline int pci_extract_slot(u32int device) {
	return (u8int)((device >> 8));
}
static inline int pci_extract_func(u32int device) {
	return (u8int)(device);
}

static inline u32int pci_get_addr(u32int device, int field) {
	return 0x80000000 | (pci_extract_bus(device) << 16) | (pci_extract_slot(device) << 11) | (pci_extract_func(device) << 8) | ((field) & 0xFC);
}

static inline u32int pci_box_device(int bus, int slot, int func) {
	return (u32int)((bus << 16) | (slot << 8) | func);
}

u32int pci_read_field(u32int device, int field, int size);
void pci_write_field(u32int device, int field, int size, u32int value);
u16int pci_find_type(u32int dev);
const char * pci_vendor_lookup(unsigned short vendor_id);
const char * pci_device_lookup(unsigned short vendor_id, unsigned short device_id);
void pci_scan_hit(pci_func_t f, u32int dev, void * extra);
void pci_scan_func(pci_func_t f, int type, int bus, int slot, int func, void * extra);
void pci_scan_slot(pci_func_t f, int type, int bus, int slot, void * extra);
void pci_scan_bus(pci_func_t f, int type, int bus, void * extra);
void pci_scan(pci_func_t f, int type, void * extra);

typedef struct {
	unsigned long Magic;
	unsigned long EntryPoint;
	unsigned char Version;
	unsigned char Pages;
	unsigned char CRC;
} PCI_BIOS_t;

typedef struct {
	unsigned short VendorID;
	unsigned short DeviceID;
	unsigned short CommandReg;
	unsigned short StatusReg;
	unsigned char RevisionID;
	unsigned char ProgIF;
	unsigned char SubClass;
	unsigned char ClassCode;
	unsigned char CachelineSize;
	unsigned char Latency;
	unsigned char HeaderType;
	unsigned char BIST;

	unsigned int BAR0; //Base Address Register
	unsigned int BAR1;
	unsigned int BAR2;
	unsigned int BAR3;
	unsigned int BAR4;
	unsigned int BAR5;
	unsigned int CardbusCISPtr;
	unsigned short SubVendorID; //Subsystem
	unsigned short SubDeviceID;
	unsigned int ExRomAddress; //Expansion ROM
	unsigned int Reserved1;
	unsigned int Reserved2;
	unsigned char IRQ; //IRQ number
	unsigned char PIN; //IRQ PIN number
	unsigned char MinGrant;
	unsigned char MaxLatency;
} PCI_Device_t;

typedef struct {
	unsigned short VendorID;
	unsigned short DeviceID;
	unsigned int BAR0;
	unsigned int BAR1;
	unsigned int BAR2;
	unsigned int BAR3;
	unsigned int BAR4;
	unsigned int BAR5;
	unsigned char IRQ;
} PCI_Args_t;

typedef struct PCI_common
{
	unsigned short vendor_id;
	unsigned short device_id;
	unsigned short cmd_reg;
	unsigned short status_reg;
	unsigned char revision_id;
	unsigned char Prog_if;
	unsigned char subclass;
	unsigned char classcode;
	unsigned char cacheline_size;
	unsigned char latency;
	unsigned char header_type;
	unsigned char BIST;		//self test
}__attribute((packed)) PCI_common;

struct pci_dev
{
	unsigned int bus,dev,func;
	PCI_common *common;
	unsigned char irq;
	unsigned int devi[60];
	struct pci_dev *next;
	struct pci_dev *prev;
};

typedef struct confadd
{
    unsigned char reg:8;
    unsigned char func:3;
    unsigned char dev:5;
    unsigned char bus:8;
    unsigned char rsvd:7;
    unsigned char enable:1;
} confadd_t;

// Initialise PCI
void init_PCI();
#endif//PCI_H
