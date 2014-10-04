//MatrixOS-elf.h
#ifndef ELF_H
#define ELF_H
#include "stdio.h"

void elf(char* file);


//offsets as comment
typedef u32int	Elf32_Addr; // Program address //4
typedef u32int	Elf32_Off;  // File offset		//4
typedef u16int	Elf32_Half;//2
typedef u32int	Elf32_Word;//4
typedef s32int	Elf32_Sword;//4


#define ELF32_FSZ_ADDR	4
#define ELF32_FSZ_HALF	2
#define ELF32_FSZ_OFF	4
#define ELF32_FSZ_SWORD	4
#define ELF32_FSZ_WORD	4

#define EI_NIDENT		16
typedef struct {
	unsigned char	e_ident[EI_NIDENT];//0x74, 'E', 'L', 'F', ...
	Elf32_Half	e_type;
	Elf32_Half	e_machine;
	Elf32_Word	e_version;
	Elf32_Addr	e_entry;
	Elf32_Off	e_phoff;
	Elf32_Off	e_shoff;
	Elf32_Word	e_flags;
	Elf32_Half	e_ehsize;
	Elf32_Half	e_phentsize;
	Elf32_Half	e_phnum;
	Elf32_Half	e_shentsize;
	Elf32_Half	e_shnum;
	Elf32_Half	e_shstrndx;
}Elf32_Ehdr;

#define ELFMAG0			0x7f
#define ELFMAG1			’E’
#define ELFMAG2			’L’
#define ELFMAG3			’F’
#define ELFMAG			"\177ELF"
#define SELFMAG			4

#define EI_MAG0			0
#define EI_MAG1			1
#define EI_MAG2			2
#define EI_MAG3			3
#define EI_CLASS		4
#define EI_DATA			5
#define EI_VERSION		6
#define EI_PAD			7

#define ELFCLASSNONE	0
#define ELFCLASS32		1
#define ELFCLASS64		2
#define ELFCLASSNUM		3

#define ELFDATANONE		0
#define ELFDATA2LSB		1
#define ELFDATA2MSB		2
#define ELFDATANUM		3

#define ET_NONE			0
#define ET_REL			1
#define ET_EXEC			2
#define ET_DYN			3
#define ET_CORE			4
#define ET_NUM			5
#define ET_LOPROC		0xff00
#define ET_HIPROC		0xffff
#define EM_NONE			0
#define EM_M32			1
#define EM_SPARC		2
#define EM_386			3
#define EM_68K			4
#define EM_88K			5
#define EM_486			6
#define EM_860			7
#define EM_NUM			8
#define EV_NONE		0
#define EV_CURRENT	1
#define EV_NUM		2
typedef struct {
	Elf32_Word	p_type;
	Elf32_Off	p_offset;
	Elf32_Addr	p_vaddr;
	Elf32_Addr	p_paddr;
	Elf32_Word	p_filesz;
	Elf32_Word	p_memsz;
	Elf32_Word	p_flags;
	Elf32_Word	p_align;
} Elf32_Phdr;
#define PT_NULL		0
#define PT_LOAD		1
#define PT_DYNAMIC	2
#define PT_INTERP	3
#define PT_NOTE		4
#define PT_SHLIB	5
#define PT_PHDR		6
#define PT_NUM		7
#define PT_LOPROC	0x70000000
#define PT_HIPROC	0x7fffffff
#define PF_R		0x4
#define PF_W		0x2
#define PF_X		0x1
#define PF_MASKPROC	0xf0000000

typedef struct {
	Elf32_Word	sh_name;
	Elf32_Word	sh_type;
	Elf32_Word	sh_flags;
	Elf32_Addr	sh_addr;
	Elf32_Off	sh_offset;
	Elf32_Word	sh_size;
	Elf32_Word	sh_link;
	Elf32_Word	sh_info;
	Elf32_Word	sh_addralign;
	Elf32_Word	sh_entsize;
} Elf32_Shdr;
#define SHT_NULL		0
#define SHT_PROGBITS	1
#define SHT_SYMTAB		2
#define SHT_STRTAB		3
#define SHT_RELA		4
#define SHT_HASH		5
#define SHT_DYNAMIC		6
#define SHT_NOTE		7
#define SHT_NOBITS		8
#define SHT_REL			9
#define SHT_SHLI		10
#define SHT_DYNSYM		11
#define SHT_NUM			12
#define SHT_LOUSER		0x80000000
#define SHT_HIUSER		0xffffffff
#define SHT_LOPROC		0x70000000
#define SHT_HIPROC		0x7fffffff
#define SHF_MASKPROC	0xf0000000
#define SHF_WRITE		0x1
#define SHF_ALLOC		0x2
#define SHF_EXECINSTR	0x4

#define SHN_UNDEF		0
#define SHN_LORESERVE	0xff00
#define SHN_ABS			0xfff1
#define SHN_COMMON		0xfff2
#define SHN_HIRESERVE	0xffff
#define SHN_LOPROC		0xff00
#define SHN_HIPROC		0xff1f
typedef struct {
	Elf32_Word st_name;
	Elf32_Addr st_value;
	Elf32_Word st_size;
	unsigned char st_info;
	unsigned char st_other;
	Elf32_Half st_shndx;
} Elf32_Sym;
#define STN_UNDEF0
#define ELF32_ST_BIND(info)				((info)>>4)
#define ELF32_ST_TYPE(info)				((info)&0xf)
#define ELF32_ST_INFO(bind,type)		(((bind)<<4)+((type)&0xf))
#define STB_LOCAL	0
#define STB_GLOBAL	1
#define STB_WEAK	2
#define STB_NUM		3
#define STB_LOPROC	13
#define STB_HIPROC	15

#define STT_NOTYPE0
#define STT_OBJECT1
#define STT_FUNC2
#define STT_SECTION3
#define STT_FILE4
#define STT_NUM5
#define STT_LOPROC13
#define STT_HIPROC15
typedef struct {
	Elf32_Addr r_offset;
	Elf32_Word r_info;
} Elf32_Rel;
typedef struct {
	Elf32_Addr r_offset;
	Elf32_Word r_info;
	Elf32_Sword r_addend;
} Elf32_Rela;
#define ELF32_R_SYM(info)		((info)>>8)
#define ELF32_R_TYPE(info)		((unsignedchar)(info))
#define ELF32_R_INFO(sym,type)	(((sym)<<8)+(unsignedchar)(type)

#endif//ELF_H
