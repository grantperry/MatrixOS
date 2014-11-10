#include <drivers/ata.h>
#include <isr.h>

u8int ata_primary_interupt = 0;
u8int ata_secondary_interupt = 0;

void ata_set_bus (u8int bus) {
	u8int out = 0;
	if (bus = 0) { out = 0xA0; }
	if (bus = 1) { out = 0xB0; }
	outb(ATA_MASTER_BUS, out);
}

u8int ata_read_status() {
	inb(ATA_STATUS_IO); //OSDEV says read register 5 times and only remember the 5th...
	inb(ATA_STATUS_IO);
	inb(ATA_STATUS_IO);
	inb(ATA_STATUS_IO);
	u8int out = inb(ATA_STATUS_IO);
	return out;
}

void init_ata_pio() {
	serialf("[ATA][PIO] Starting\n");
	register_ata_interupts();
	
	/*ata_set_bus(0); //set bus 0
	
	outb(Sectorcount, 0); //zero out these registers
	outb(LBAhi, 0);
	outb(LBAmid, 0);
	outb(LBAlo, 0);
	
	outb(ATA_COMMAND_IO, 0xEC); //send IDENTIFY command
	
	//u8int ata_stat = ata_read_status();
	u8int ata_stat = inb(ATA_STATUS_IO); //read status register
	
	if(ata_stat == 0) {
		serialf("[ATA] No Drives Found. %d", ata_stat);
		return;
	}*/
	
	u8int slavebit = 0;
	u8int LBA = 0;
	u8int count = 0;
	
	outb(0x1F6, 0xE0 | (slavebit << 4) | ((LBA >> 24) & 0x0F));
	outb(0x1F1, 0x00);
	outb(0x1F2, (unsigned char) count);
	outb(0x1F3, (unsigned char) LBA);
	outb(0x1F4, (unsigned char)(LBA >> 8));
	outb(0x1F5, (unsigned char)(LBA >> 16));
	outb(0x1F7, 0x20);
	
}


void interupt_primary_ata(registers_t* regs) {
	serialf("[ATA][PRIMARY] Interupt recieved\n");
	ata_primary_interupt = 1;
}

void interupt_secondary_ata(registers_t* regs) {
	serialf("[ATA][SECONDARY] Interupt recieved\n");
	ata_secondary_interupt = 1;
}

void register_ata_interupts() {
	register_interrupt_handler ( IRQ14, interupt_primary_ata );
	register_interrupt_handler ( IRQ15, interupt_secondary_ata );
	
}
