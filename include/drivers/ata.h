#ifndef ATA_H
#define ATA_H

#define ATA_MASTER_BUS	0x1F6
#define ATA_COMMAND_IO	0x1F7
#define ATA_STATUS_IO	0x1F7
#define Sectorcount		0x1F2
#define	LBAlo			0x1F3
#define LBAmid			0x1F4
#define LBAhi			0x1F5


void init_ata_pio();
void register_ata_interupts();

#endif//ATA_H
