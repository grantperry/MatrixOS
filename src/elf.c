#include "elf.h"
#include "fs.h"
#include "paging.h"

u32int maxsz = 0x10000;


void *elf_lookup_symbol(const char *name) {
	return 0;
}
static inline Elf32_Shdr *elf_sheader(Elf32_Ehdr *hdr) {
	return (Elf32_Shdr *)((int)hdr + hdr->e_shoff);
}
 
static inline Elf32_Shdr *elf_section(Elf32_Ehdr *hdr, int idx) {
	return &elf_sheader(hdr)[idx];
}


u32int elf_check_file(Elf32_Ehdr *hdr) {
	if(!hdr) return 0;
	if(hdr->e_ident[EI_MAG0] != 0x7F) {
		serialf("[ELF] Header EI_MAG0 incorrect.\n");
		return 0;
	}
	if(hdr->e_ident[EI_MAG1] != 'E') {
		serialf("[ELF] Header EI_MAG1 incorrect.\n");
		return 0;
	}
	if(hdr->e_ident[EI_MAG2] != 'L') {
		serialf("[ELF] Header EI_MAG2 incorrect.\n");
		return 0;
	}
	if(hdr->e_ident[EI_MAG3] != 'F') {
		serialf("[ELF] Header EI_MAG3 incorrect.\n");
		return 0;
	}
	return 1;
}

u32int elf_check_supported(Elf32_Ehdr *hdr) {
	if(!elf_check_file(hdr)) {
		serialf("[ELF] Invalid ELF File.\n");
		return 0;
	}
	if(hdr->e_ident[EI_CLASS] != ELFCLASS32) {
		serialf("[ELF] Unsupported ELF File Class.\n");
		return 0;
	}
	if(hdr->e_ident[EI_DATA] != ELFDATA2LSB) {
		serialf("[ELF] Unsupported ELF File byte order.\n");
		return 0;
	}
	if(hdr->e_machine != EM_386) {
		serialf("[ELF] Unsupported ELF File target.\n");
		return 0;
	}
	if(hdr->e_ident[EI_VERSION] != EV_CURRENT) {
		serialf("[ELF] Unsupported ELF File version.\n");
		return 0;
	}
	if(hdr->e_type != ET_REL && hdr->e_type != ET_EXEC) {
		serialf("[ELF] Unsupported ELF File type.\n");
		return 0;
	}
	return 1;
}

void elf_print_header(Elf32_Ehdr *elf) {
	
	if(elf_check_file(elf)) {
		serialf("[ELF] Magic Correct!\n");
	}
	serialf("[ELF][TYPE] ");
	switch(elf->e_type) {
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
	switch(elf->e_machine)
	{
		case EM_NONE:
			serialf("None (0x0)\n");
			break;

		case EM_386:
			serialf("INTEL x86 (%h)\n", EM_386);
			break;

		default:
			serialf("Machine = %h\n", elf->e_machine);
			break;
	}
	
	serialf("[ELF] Entry point\t\t= %h\n", elf->e_entry);

	/* ELF header size in bytes */
	serialf("[ELF] header size\t\t= %h\n", elf->e_ehsize);

	/* Program Header */
	serialf("\n[ELF] Program Header\t\t= ");
	serialf("%h\n", elf->e_phoff);		/* start */
	serialf("\t\t\tentries\t= %d\n", elf->e_phnum);	/* num entry */
	serialf("\t\t\tbytes\t= %d\n", elf->e_phentsize);	/* size/entry */

	/* Section header starts at */
	serialf("\n[ELF] Section Header\t\t= ");
	serialf("%h\n", elf->e_shoff);		/* start */
	serialf("\t\t\tentries = %d\n", elf->e_shnum);	/* num entry */
	serialf("\t\t\tbytes\t= %d\n", elf->e_shentsize);	/* size/entry */
	serialf("\tstring table offset\t= %h\n", elf->e_shstrndx);

	/* File flags (Machine specific)*/
	serialf("\n[ELF] File flags\t\t= 0x%08x\n", elf->e_flags);
}

char *read_section(char *fd, Elf32_Shdr* sh)
{
	char* buff = (char*)kmalloc(sh->sh_size);
	if(!buff) {
		printf("[ELF] Failed to allocate %dbytes\n", sh->sh_size);
	}

	memcpy(buff, fd + sh->sh_offset, sh->sh_size);
	serialf("LOOK: %s", buff);
	return buff;
}




static inline char *elf_str_table(Elf32_Ehdr *hdr) {
	if(hdr->e_shstrndx == SHN_UNDEF) return 0;
	return (char *)hdr + elf_section(hdr, hdr->e_shstrndx)->sh_offset;
}
 
static inline char *elf_lookup_string(Elf32_Ehdr *hdr, int offset) {
	char *strtab = elf_str_table(hdr);
	if(strtab == 0) return 0;
	return strtab + offset;
}

static int elf_get_symval(Elf32_Ehdr *hdr, int table, u32int idx) {
	if(table == SHN_UNDEF || idx == SHN_UNDEF) return 0;
	Elf32_Shdr *symtab = elf_section(hdr, table);
 
	if(idx >= symtab->sh_size) {
		serialf("[ELF] Symbol Index out of Range (%d:%u).\n", table, idx);
		return ELF_RELOC_ERR;
	}
 
	int symaddr = (int)hdr + symtab->sh_offset;
	Elf32_Sym *symbol = &((Elf32_Sym *)symaddr)[idx];
	if(symbol->st_shndx == SHN_UNDEF) {
		// External symbol, lookup value
		Elf32_Shdr *strtab = elf_section(hdr, symtab->sh_link);
		const char *name = (const char *)hdr + strtab->sh_offset + symbol->st_name;
 
		extern void *elf_lookup_symbol(const char *name);
		void *target = elf_lookup_symbol(name);
 
		if(target == 0) {
			// Extern symbol not found
			if(ELF32_ST_BIND(symbol->st_info) & STB_WEAK) {
				// Weak symbol initialized as 0
				return 0;
			} else {
				serialf("[ELF] Undefined External Symbol : %s.\n", name);
				return ELF_RELOC_ERR;
			}
		} else {
			return (int)target;
		}
		} else if(symbol->st_shndx == SHN_ABS) {
		// Absolute symbol
		return symbol->st_value;
	} else {
		// Internally defined symbol
		Elf32_Shdr *target = elf_section(hdr, symbol->st_shndx);
		return (int)hdr + symbol->st_value + target->sh_offset;
	}
}

static int elf_do_reloc(Elf32_Ehdr *hdr, Elf32_Rel *rel, Elf32_Shdr *reltab) {
	Elf32_Shdr *target = elf_section(hdr, reltab->sh_info);
 
	int addr = (int)hdr + target->sh_offset;
	int *ref = (int *)(addr + rel->r_offset);
	// Symbol value
	int symval = 0;
	if(ELF32_R_SYM(rel->r_info) != SHN_UNDEF) {
		symval = elf_get_symval(hdr, reltab->sh_link, ELF32_R_SYM(rel->r_info));
		if(symval == ELF_RELOC_ERR) return ELF_RELOC_ERR;
	}
	// Relocate based on type
	switch(ELF32_R_TYPE(rel->r_info)) {
		case R_386_NONE:
			// No relocation
			break;
		case R_386_32:
			// Symbol + Offset
			*ref = DO_386_32(symval, *ref);
			break;
		case R_386_PC32:
			// Symbol + Offset - Section Offset
			*ref = DO_386_PC32(symval, *ref, (int)ref);
			break;
		default:
			// Relocation type not supported, display error and return
			serialf("[ELF] Unsupported Relocation Type (%d).\n", ELF32_R_TYPE(rel->r_info));
			return ELF_RELOC_ERR;
	}
	return symval;
}

static int elf_load_stage1(Elf32_Ehdr *hdr) {
	Elf32_Shdr *shdr = elf_sheader(hdr);
 
	unsigned int i;
	// Iterate over section headers
	for(i = 0; i < hdr->e_shnum; i++) {
		Elf32_Shdr *section = &shdr[i];
 
		// If the section isn't present in the file
		if(section->sh_type == SHT_NOBITS) {
			// Skip if it the section is empty
			if(!section->sh_size) continue;
			// If the section should appear in memory
			if(section->sh_flags & SHF_ALLOC) {
				// Allocate and zero some memory
				void *mem = (void*)kmalloc(section->sh_size);
				memset(mem, 0, section->sh_size);
 
				// Assign the memory offset to the section offset
				section->sh_offset = (int)mem - (int)hdr;
				serialf("[ELF] Allocated memory for a section (%d).\n", section->sh_size);
			}
		}
	}
	return 0;
}

static int elf_load_stage2(Elf32_Ehdr *hdr) {
	Elf32_Shdr *shdr = elf_sheader(hdr);
 
	unsigned int i, idx;
	// Iterate over section headers
	for(i = 0; i < hdr->e_shnum; i++) {
		Elf32_Shdr *section = &shdr[i];
 
		// If this is a relocation section
		if(section->sh_type == SHT_REL) {
			// Process each entry in the table
			for(idx = 0; idx < section->sh_size / section->sh_entsize; idx++) {
				Elf32_Rel *reltab = &((Elf32_Rel *)((int)hdr + section->sh_offset))[idx];
				int result = elf_do_reloc(hdr, reltab, section);
				// On error, display a message and return
				if(result == ELF_RELOC_ERR) {
					serialf("[ELF] Failed to relocate symbol.\n");
					return ELF_RELOC_ERR;
				}
			}
		}
	}
	return 0;
}

static inline void *elf_load_rel(Elf32_Ehdr *hdr) {
	int result;
	result = elf_load_stage1(hdr);
	if(result == ELF_RELOC_ERR) {
		serialf("[ELF] Unable to load ELF file.\n");
		return 0;
	}
	result = elf_load_stage2(hdr);
	if(result == ELF_RELOC_ERR) {
		serialf("[ELF] Unable to load ELF file.\n");
		return 0;
	}
	// TODO : Parse the program header (if present)
	return (void *)hdr->e_entry;
}

void elf_load_exec (char *elf, Elf32_Ehdr *elf_Ehdr) {
	u32int phoff = (u32int)elf + elf_Ehdr->e_phoff;
	u32int shoff = (u32int)elf + elf_Ehdr->e_shoff;
	
	Elf32_Shdr *elf_Shdr = (Elf32_Shdr*)(shoff + (0 * elf_Ehdr->e_shentsize)); //string table
	u32int nametbl =  (u32int)elf + (elf_Shdr->sh_offset);
	
	Elf32_Phdr *elf_Phdr = (Elf32_Phdr*)(phoff + (0 * elf_Ehdr->e_phentsize));
	
	virtual_map_pages ( elf_Phdr->p_vaddr, 0x1000, 1, 1 ); //TODO make real size
	u32int i = 0;
	while (i < elf_Ehdr->e_phnum) {
		elf_Phdr = (Elf32_Phdr*)(phoff + (i * elf_Ehdr->e_phentsize));
		void *mem = (void*)elf_Phdr->p_offset;
		memcpy(mem, elf + elf_Phdr->p_offset, elf_Phdr->p_filesz);
		
		
		
		
		
		
		
		i++;
	}
	i = 0;
	
	//asm volatile("call 0x8048080");
}

void *elf_load_file(char *file) {
	Elf32_Ehdr *hdr = (Elf32_Ehdr *)file;
	if(!elf_check_supported(hdr)) {
		serialf("[ELF] File cannot be loaded.\n");
		return;
	}
	switch(hdr->e_type) {
		case ET_EXEC:
			serialf("[ELF] Load Executable");
			elf_load_exec(file, hdr);
			return 0;
		case ET_REL:
			serialf("[ELF] Load Relative\n");
			return elf_load_rel(hdr);
	}
	return 0;
}

void elf_print_sections(char *elf, Elf32_Ehdr *elf_Ehdr) {
	serialf("|ELF Sections|====================================================================================\n");
	serialf("|idx\tname\t\ttype\t\tflags\taddress\toffset\tsize\taddress align\tentry size\n");
	serialf("|-------------------------------------------------------------------------------------------------\n");
	u32int shoff = (u32int)elf + elf_Ehdr->e_shoff;
	Elf32_Shdr *elf_Shdr = (Elf32_Shdr*)(shoff + (elf_Ehdr->e_shstrndx * elf_Ehdr->e_shentsize)); //string table
	u32int nametbl =  (u32int)elf + (elf_Shdr->sh_offset);

	u32int i = 0;
	for (i = 0; i < elf_Ehdr->e_shnum; i++) {
		elf_Shdr = (Elf32_Shdr*)(shoff + (i * elf_Ehdr->e_shentsize));
		
		serialf("|%d\t", i);
		serialf( ((nametbl) + elf_Shdr->sh_name));
		serialf("\t\t");
		serialf("%h\t", elf_Shdr->sh_type);
		serialf("%h\t", elf_Shdr->sh_flags);
		serialf("%h\t", elf_Shdr->sh_addr);
		serialf("%h\t", elf_Shdr->sh_offset);
		serialf("%h\t", elf_Shdr->sh_size);
		serialf("%h\t", elf_Shdr->sh_addralign);
		serialf("%h\t", elf_Shdr->sh_entsize);
		
		serialf("\n");
		elf_Shdr = 0;
	}
	serialf("|-------------------------------------------------------------------------------------------------\n");
}


elf_print_program(char *elf, Elf32_Ehdr *elf_Ehdr) {
	serialf("|ELF  Program Headers|================================================================\n");
	serialf("|idx\ttype\toffset\tvirt add\tpys add\t\tfile sz\tmem sz\tflags\talign\n");
	serialf("|-------------------------------------------------------------------------------------\n");
	
	u32int phoff = (u32int)elf + elf_Ehdr->e_phoff;
	
	u32int i;
	for(i = 0; i < elf_Ehdr->e_phnum; i++) {
		Elf32_Phdr *elf_Phdr = (Elf32_Phdr*)(phoff + (i * elf_Ehdr->e_phentsize));
		serialf("|%d\t", i);
		serialf("%d\t", elf_Phdr->p_type);
		serialf("%h\t", elf_Phdr->p_offset);
		serialf("%h\t", elf_Phdr->p_vaddr);
		serialf("%h\t", elf_Phdr->p_paddr);
		serialf("%h\t", elf_Phdr->p_filesz);
		serialf("%h\t", elf_Phdr->p_memsz);
		u8int r = 0, w = 0, x = 0;
		r = elf_Phdr->p_flags & PF_R;
		w = elf_Phdr->p_flags & PF_W;
		x = elf_Phdr->p_flags & PF_X;
		serialf("%s",   (r) ? "-" : "R");
		serialf("%s",   (w) ? "-" : "W");
		serialf("%s\t", (x) ? "-" : "E");
		serialf("%h\t", elf_Phdr->p_align);
		
		serialf("\n");
		elf_Phdr = 0;
	}
	serialf("|-------------------------------------------------------------------------------------\n");
}


u8int elf(char* file) {
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
	
	Elf32_Ehdr *elf_Ehdr = (Elf32_Ehdr*)kmalloc(sizeof(Elf32_Ehdr));
	memcpy(elf_Ehdr, elf, sizeof(Elf32_Ehdr)); //ofset 0
	elf_print_header(elf_Ehdr);
	
	if(elf_check_file(elf_Ehdr)) {
		serialf("[ELF] Magic Correct!\n");
	} else {
		return 0;
	}
	if(elf_check_supported(elf_Ehdr)) {
		serialf("[ELF] Specs Correct!\n");
	} else {
		return 0;
	}
	
	if( elf_Ehdr->e_type == ET_EXEC) {
		elf_print_program(elf, elf_Ehdr);
	}
	
	elf_print_sections(elf, elf_Ehdr);
	
	elf_load_file(elf);
	
	
}
