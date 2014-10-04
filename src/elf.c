#include "elf.h"
#include "fs.h"
#include "paging.h"

u32int maxsz = 0x10000;

void elf_identify(char *elf) {
	Elf32_Ehdr *elf_Ehdr = (Elf32_Ehdr*)elf; //ofset 0
	if (elf_Ehdr->e_ident[EI_MAG0] != 0x7F) {
		serialf("[ELF] Magic number incorrect\n");
		return;
	}
	if (elf_Ehdr->e_ident[EI_MAG1] != 'E') {
		serialf("[ELF] Magic number incorrect\n");
		return;
	}
	if (elf_Ehdr->e_ident[EI_MAG2] != 'L') {
		serialf("[ELF] Magic number incorrect\n");
		return;
	}
	if (elf_Ehdr->e_ident[EI_MAG3] != 'F') {
		serialf("[ELF] Magic number incorrect\n");
		return;
	}
	serialf("[ELF] Magic Correct\n");
	serialf("[ELF][TYPE] ");
	switch(elf_Ehdr->e_type) {
		case ET_NONE:
			serialf("NONE\n");
			break;
		case ET_REL:
			serialf("RELOCATOBAL\n");
			break;
		case ET_EXEC:
			serialf("EXECUTABLE\n");
			break;
		case ET_DYN:
			serialf("DYNAMIC\n");
			break;
		case ET_CORE:
			serialf("CORE\n");
			break;
		case ET_NUM:
			serialf("NUM\n");
			break;
		default:
			serialf("UNKNOWN\n");
			return;
	}
	
	serialf("[ELF][MACHINE] ");
	switch(elf_Ehdr->e_machine)
	{
		case EM_NONE:
			serialf("None (0x0)\n");
			break;

		case EM_386:
			serialf("INTEL x86 (%h)\n", EM_386);
			break;

		default:
			serialf("Machine\t= %h\n", elf_Ehdr->e_machine);
			break;
	}
	
	serialf("Entry point\t= 0x%08x\n", elf_Ehdr->e_entry);

	/* ELF header size in bytes */
	serialf("ELF header size\t= 0x%08x\n", elf_Ehdr->e_ehsize);

	/* Program Header */
	serialf("\nProgram Header\t= ");
	serialf("%h\n", elf_Ehdr->e_phoff);		/* start */
	serialf("\t\t  %d entries\n", elf_Ehdr->e_phnum);	/* num entry */
	serialf("\t\t  %d bytes\n", elf_Ehdr->e_phentsize);	/* size/entry */

	/* Section header starts at */
	serialf("\nSection Header\t= ");
	serialf("%h\n", elf_Ehdr->e_shoff);		/* start */
	serialf("\t\t  %d entries\n", elf_Ehdr->e_shnum);	/* num entry */
	serialf("\t\t  %d bytes\n", elf_Ehdr->e_shentsize);	/* size/entry */
	serialf("\t\t %h (string table offset)\n", elf_Ehdr->e_shstrndx);

	/* File flags (Machine specific)*/
	serialf("\nFile flags \t= 0x%08x\n", elf_Ehdr->e_flags);
} 


void elf(char* file) {
	serialf("\n[ELF] Loading \"%s\".\n", file);
	FILE *elff = ( FILE* ) f_open ( file, fs_root, "r" );
	u32int elffsize = f_read ( elff, 0, maxsz, 0 );
	char *elf = ( char* ) kmalloc ( sizeof(char) * elffsize );
	serialf("[ELF] Buffer created of size\t %h\n", elffsize);
	u32int elffsize1 = f_read ( elff, 0, elffsize, elf);
	if (elffsize != elffsize1) {
		serialf("[ELF] File Error!\n");
		return;
	}
	elf_identify(elf);
}
