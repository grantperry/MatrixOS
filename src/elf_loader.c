/*-----------------------------------------------------------------------------
 *
 *		ELF executable loader
 *		(c) maisvendoo, 30.08.2013
 *
 *---------------------------------------------------------------------------*/

#include	"elf_loader.h"

extern fs_node_t* fs_root;

/*-----------------------------------------------------------------------------
 *
 *---------------------------------------------------------------------------*/
elf_sections_t* load_elf ( char* name ) {
	u32int sz = 0;

	/* Allocate ELF file structure */
	elf_sections_t* elf = ( elf_sections_t* ) kmalloc_a ( sizeof ( elf_sections_t ) );

	/* Open ELF file */
	FILE* file_elf = fopen ( fs_root, name, "r" );

	printf ( "[load_elf] Called for inode  -  bytes of data allocated\n", file_elf->inode );

	/* Read ELF header */
	elf->elf_header = ( Elf32_Ehdr* ) kmalloc_a ( sizeof ( Elf32_Ehdr ) );

	sz = fread ( file_elf, elf->elf_header, sizeof ( Elf32_Ehdr ) );

	/* Read program header */
	Elf32_Half proc_entries = elf->elf_header->e_phnum;

	elf->p_header = ( Elf32_Phdr* ) kmalloc_a ( sizeof ( Elf32_Phdr ) *proc_entries );

	//fseek ( file_elf, elf->elf_header->e_phoff );

	//sz = fread ( file_elf, elf->p_header, sizeof ( Elf32_Phdr ) *proc_entries );

	/* Read ELF sections */
	//Elf32_Half sec_entries = elf->elf_header->e_shnum;

	//elf->section = ( Elf32_Shdr* ) kmalloc_a ( sizeof ( Elf32_Shdr ) *sec_entries );

	//fseek ( file_elf, elf->elf_header->e_shoff );

	//sz = fread ( file_elf, elf->section, sizeof ( Elf32_Shdr ) *sec_entries );

	//elf->file = file_elf;

	//return elf;
}
