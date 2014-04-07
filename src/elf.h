/*-----------------------------------------------------------------------------
 *
 * 		ELF executable format's structures
 * 		(c) maisvendoo, 30.08.2013
 *
 *---------------------------------------------------------------------------*/
#ifndef		ELF_H
#define		ELF_H

#define		EI_NIDENT	16

/*-----------------------------------------------------------------------------
 *		Section types
 *---------------------------------------------------------------------------*/
#define		SHT_NULL		0
#define		SHT_PROGBITS	1
#define		SHT_SYMTAB		2
#define		SHT_STRTAB		3
#define		SHT_RELA		4
#define		SHT_HASH		5
#define		SHT_DYNAMIC		6
#define		SHT_NOTE		7
#define		SHT_NOBITS		8
#define		SHT_REL			9
#define		SHT_SHLIB		10
#define		SHT_DYNSYM		11
#define		SHT_LOPROC		0x70000000
#define		SHT_HIPROC		0x7FFFFFFF
#define		SHT_LOUSER		0x80000000
#define		SHT_HIUSER		0xFFFFFFFF

/*-----------------------------------------------------------------------------
 *		Section types
 *---------------------------------------------------------------------------*/
#define		SHF_WRITE		0x1
#define		SHF_ALLOC		0x2
#define		SHF_EXECINSTR	0x4
#define		SHF_MASKPROC	0xF0000000

/*-----------------------------------------------------------------------------
 *		Types of object files
 *---------------------------------------------------------------------------*/
#define		ET_NONE			0			/* No file type */
#define		ET_REL			1			/* Relocatable file */
#define		ET_EXEC			2			/* Executable file */
#define		ET_DYN			3			/* Shared object file */
#define		ET_CORE			4			/* Core file */
#define		ET_LOPROC		0xFF00		/* Processor-specific */
#define		ET_HIPROC		0xFFFF		/* Processor-specific */

/*-----------------------------------------------------------------------------
 *
 *---------------------------------------------------------------------------*/
#define		EM_NONE			0
#define		EM_M32			1
#define		EM_SPARC		2
#define		EM_386			3
#define		EM_68K			4
#define		EM_88K			5
#define		EM_860			7
#define		EM_MIPS			8

/*-----------------------------------------------------------------------------
 *
 *---------------------------------------------------------------------------*/
#define		EV_NONE			0
#define		EV_CURRENT		1

/*-----------------------------------------------------------------------------
 *
 *---------------------------------------------------------------------------*/
#define		PT_NULL			0
#define		PT_LOAD			1
#define		PT_DYNAMIC		2
#define		PT_INTERP		3
#define		PT_NOTE			4
#define		PT_SHLIB		5
#define		PT_PHDR			6
#define		PT_LOPROC		0x70000000
#define		PT_HIPROC		0x7FFFFFFF

/*-----------------------------------------------------------------------------
 * 		Data types
 *---------------------------------------------------------------------------*/
typedef		unsigned int		Elf32_Addr;
typedef		unsigned short int	Elf32_Half;
typedef		unsigned int		Elf32_Off;
typedef		int					Elf32_Sword;
typedef		unsigned int		Elf32_Word;

/*-----------------------------------------------------------------------------
 * 		ELF header
 *---------------------------------------------------------------------------*/
typedef	struct {
	unsigned char	ident[EI_NIDENT];	/* ELF identification data */
	Elf32_Half		type;				/* Type of object file */
	Elf32_Half		mashine;			/* Architecture type  */
	Elf32_Word		version;			/* Object file version  */
	Elf32_Addr		entry;				/* Entry point to process */
	Elf32_Off		phoff;				/* Program header offset */
	Elf32_Off		shoff;				/* Section table header offset */
	Elf32_Word		flags;				/* Processor-specific flags */
	Elf32_Half		ehsize;				/* ELF header size */
	Elf32_Half		phentsize;			/* Size of program header entry */
	Elf32_Half		phnum;				/* Number of program header entries */
	Elf32_Half		shentsize;			/* Section header entry size */
	Elf32_Half		shnum;				/* Section header entries number */
	Elf32_Half		shstrndx;			/*  */
} Elf32_Ehdr;

/*-----------------------------------------------------------------------------
 * 		Section header
 *---------------------------------------------------------------------------*/
typedef	struct {
	Elf32_Word	name;					/* Index into section header stirng table */
	Elf32_Word	type;					/* Section type */
	Elf32_Word	flags;					/* Section attribute flags */
	Elf32_Addr	addr;					/* Section virtual address in process image */
	Elf32_Off	offset;					/* Section offset in file */
	Elf32_Word	size;					/* Section size */
	Elf32_Word	link;					/*  */
	Elf32_Word	info;					/* Extra information */
	Elf32_Word	addralign;				/* Section address alingment */
	Elf32_Word	entsize;				/* */
} Elf32_Shdr;

/*-----------------------------------------------------------------------------
 * 		Program header
 *---------------------------------------------------------------------------*/
typedef struct {
	Elf32_Word	type;					/* Program header type */
	Elf32_Off	offset;					/* Program header offset */
	Elf32_Addr	vaddr;					/* Program header virtual address */
	Elf32_Addr	paddr;
	Elf32_Word	filesz;					/* Number bytes in file image segment  */
	Elf32_Word	memsz;
	Elf32_Word	flags;
	Elf32_Word	align;
} Elf32_Phdr;


#endif
