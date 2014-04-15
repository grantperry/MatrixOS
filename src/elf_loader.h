/*-----------------------------------------------------------------------------
 *
 *		ELF executable loader
 *		(c) maisvendoo, 30.08.2013
 *
 *---------------------------------------------------------------------------*/
#ifndef		ELF_LOADER_H
#define		ELF_LOADER_H

#include	"common.h"
#include	"stdio.h"
#include	"elf.h"
#include	"kheap.h"

/*-----------------------------------------------------------------------------
 *
 *---------------------------------------------------------------------------*/
typedef struct elf_sections {
	Elf32_Ehdr*		elf_header;
	Elf32_Shdr*		section;
	Elf32_Phdr*		p_header;

	FILE*			file;

} elf_sections_t;

void start_elf ( char* name );

#endif
