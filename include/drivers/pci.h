#ifndef PCI_H
#define PCI_H

#include <common.h>

#define CONFIG_ADDRESS	0xCF8
#define CONFIG_DATA		0xCFC

void Init_PCI();

u8int readECS_BYTE(u16int);

u16int readECS_WORD(u16int);

u32int readECS_DWORD(u32int);

void writeECS_BYTE(u8int);

void writeECS_WORD(u16int);

void writeECS_DWORD(u32int);

#endif//PCI_H
