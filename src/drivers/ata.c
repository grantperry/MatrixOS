#include <drivers/ata.h>
#include <isr.h>

void init_ata_pio() {
	serialf("[ATA][PIO] Starting\n");
	register_ata_interupts();
	
}


void interupt_primary_ata(registers_t* regs) {
	serialf("[ATA][PRIMARY] Interupt recieved\n");
}

void interupt_secondary_ata(registers_t* regs) {
	serialf("[ATA][SECONDARY] Interupt recieved\n");
}

void register_ata_interupts() {
	register_interrupt_handler ( IRQ14, interupt_primary_ata );
	register_interrupt_handler ( IRQ15, interupt_secondary_ata );
	
}
