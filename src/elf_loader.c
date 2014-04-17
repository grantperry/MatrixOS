#include	"elf_loader.h"
#include 	"fs.h"

extern fs_node_t* fs_root;

u32int relocate_elf ( Elf32_Ehdr *header, u32int offset ) {
	u32int i;
	static Elf32_Phdr* pHeader;
	//static noteEntry* note;

	printf ( "[reloc_elf] %d Program Headers found, phentsize %d.\n", header->phnum, header->phentsize );

	for ( i = 0; i < header->phnum; i++ ) {
		pHeader = ( Elf32_Phdr* ) ( ( unsigned long ) header + header->phoff + i * header->phentsize );

		/* Check if its loadable */
		switch ( pHeader->type ) {

		case PT_LOAD:
			printf ( "[reloc_elf] Program header #%d Type:LOAD Offset:%h Vaddr:%h, Paddr:%h, Size:%h, MEM:%h, Align:%h!\n", i, pHeader->offset, pHeader->vaddr, pHeader->paddr, pHeader->filesz, pHeader->memsz, pHeader->align );
			virtual_map_pages(pHeader->vaddr, pHeader->filesz, 1, 1);
			memcpy((char*)pHeader->vaddr, (char*)offset + pHeader->offset, pHeader->filesz);
			
			if(pHeader->filesz < pHeader->memsz)
        {
          /* memset() the remaining memory with zeroes - stack */
          memset((char*)pHeader->vaddr + pHeader->filesz, 0, pHeader->memsz - pHeader->filesz);
        }

			break;

		case PT_DYNAMIC:
			printf ( "[reloc_elf] Program header #%d Type:DYNAMIC Addr:%h Size:%h!\n", i, pHeader->vaddr, pHeader->filesz );
			//I failed to understand what needs to be done here
			break;

		case PT_INTERP:
			printf ( "[reloc_elf] Program header #%d Type:INTERP Addr:%h Size:%h!\n", i, pHeader->vaddr, pHeader->filesz );

			//This is an external program that needs to be relocated and called.
			static unsigned char *interp;

			memcpy ( ( char* ) interp, ( char* ) offset + pHeader->offset, pHeader->filesz );

			printf ( "\tInterpreter: \"%s\"\n", interp );
			break;

		case PT_NOTE:
			printf ( "[reloc_elf] Program header #%d Type:NOTE Addr:%h Size:%h!\n", i, pHeader->vaddr, pHeader->filesz );

			//~ debug_noteEntries((u32int*)(offset+pHeader->offset));
			break;

		//case PT_SHLIB:
		//break;

		//case PT_PHDR:
		//break;

		default:
			break;
		}
	}

	return 1;
}

u32int open_elf ( char *name, u32int offset ) {
	Elf32_Ehdr *header;
	header = ( Elf32_Ehdr* ) offset; //point the header structure at the header.

	if (header->ident[0] == 0x7f && header->ident[1] == 'E' && header->ident[2] == 'L' && header->ident[3] == 'F' ) {} else {
		printf("[LoadELF] file not ELF");
		return 1;
	}
	u32int bits = 0;
	if(header->ident[4] == 1) {
		bits = 32; //32bit archtype
	}
	if(header->ident[4] == 2) {
		bits = 64; //64bit archtype
	}
	if(header->ident[4] == 0) {
		printf("[LoadELF] archtype not found!\n");
		return 1;
	}
	if(header->ident[6] != EV_CURRENT && header->version != EV_CURRENT) { //if not current version of elf
		printf("[LoadELF] ELF Version incorrect\n");
		return 1;
	}
	if(header->machine != EM_386) {
		printf("[LoadELF] ELF Machine type incorrect\n");
		return 1;
	}
	
	if ( header->ident[5] != 1 ) { //1 = little, 2 = big
		printf ( "[LoadELF] ELF architecture endianess incorrect\n" );
		return 1;
	}
	printf("[ELF] %s\'s:\n\
[ELF] Bit Type: %dbit\n\
[ELF] Version: 1\n", name, bits);

	u32int err = relocate_elf(header, offset);
	
	printf("relocate: %d\n", err);
	
	printf("[openModule_elf] Calling executable at %h\n",header->entry);
     
     /* Call the program */    
    // asm volatile("call *%0" : : "a" (header->entry));

	return 0;
}

void start_elf ( char* name ) {
	
	FILE *modulenode;

	//tests if this module exist in the root
	if ( ! ( modulenode = finddir_fs ( fs_root, name ) ) ) {
		return ;
	} else { //the module exists
		printf ( "[ELF] Found %s\n", modulenode->name );

		u32int size = modulenode->length;

		printf ( "[ELF] Reading file\n" );
		//create the actuall module buffer
		char *modulebuffer = ( char* ) kmalloc ( size );

		//open the file
		u32int modulesize = read_fs ( modulenode, 0, size, modulebuffer );
		printf ( "[ELF] size: %d\n", modulesize );

		printf ( "[ELF] Allocating space\n" );

		//create a pointer and copy the above saved buffer that address
		u32int moduleptr = kmalloc ( modulesize );
		memcpy ( ( void* ) moduleptr, ( void* ) modulebuffer, modulesize );

		printf ( "[ELF] Data moved into allocated space: %h\n", moduleptr );

		u32int elfLoadError = open_elf ( modulenode->name, moduleptr );

		if ( elfLoadError ) {
			printf ( "[load_elf] openModule_elf returned with an error: %d\n", elfLoadError );
		}

		//free stuff
		kfree ( modulebuffer );
		kfree ( ( u32int* ) moduleptr );
		//TODO f_finddir_close(modulenode);
	}
	

}
