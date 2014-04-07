/*-----------------------------------------------------------------------------
 *
 *		ELF executable loader
 *		(c) maisvendoo, 30.08.2013
 *
 *---------------------------------------------------------------------------*/

#include	"elf_loader.h"

extern fs_node_t* fs_root;

u32int relocModule_elf(Elf32_Ehdr *header, u32int offset)
{
  /*The argument u32int offset is the offset (memory location) in the OS
   * as to where the binary information should be stored */

  u32int i;
  static Elf32_Phdr* pHeader;
  //static noteEntry* note;
  
  printf("[relocModule_elf] %d Program Headers found, phentsize %d.\n", header->phnum, header->phentsize);
  
  for(i = 0; i < header->phnum; i++)
  {
    pHeader = (Elf32_Phdr*)((unsigned long) header + header->phoff + i * header->phentsize);
    
    /* Check if its loadable */
    switch(pHeader->type)
    {
     
      case PT_LOAD:
        printf("[relocModule_elf] Program header #%d Type:LOAD Offset:%h Vaddr:%h, Paddr:%h, Size:%h, MEM:%h, Align:%h!\n", i, pHeader->offset, pHeader->vaddr, pHeader->paddr, pHeader->filesz, pHeader->memsz, pHeader->align);

        //~ page_t *page;
//~ 
        //~ u32int j = pHeader->vaddr & ~0xFFF;
        //~ u32int limit = ((u32int) pHeader->vaddr + pHeader->memsz + 0x1000) & ~0xFFF;

        //Map the virtual address to an area in the stack
        //~ for(;j < limit; j+= 0x1000)
        //~ {
          //~ page = get_page(pHeader->vaddr, 1, current_directory);
          //~ alloc_frame(page, 0, 1);
          //~ printf("frame:  %h\n", page->frame);
        //~ }
        
        //Map the virtual address to an area in the stack
        virtual_map_pages(pHeader->vaddr, pHeader->filesz, TRUE, TRUE);

        printf("[relocModule_elf] pHeader->vaddr source: %h\n", offset + pHeader->offset);

        //~ memcpy((u32int*)pHeader->vaddr, (const void*)offset + pHeader->offset, pHeader->memsz);
        
        ///* Copy the program data to vaddr */
        memcpy((char*)pHeader->vaddr, (char*)offset + pHeader->offset, pHeader->filesz);
        //~ memcpy((char*)pHeader->vaddr, (char*)offset + pHeader->offset, pHeader->memsz);
        if(pHeader->filesz < pHeader->memsz)
        {
          /* memset() the remaining memory with zeroes - stack */
          memset((char*)pHeader->vaddr + pHeader->filesz, 0, pHeader->memsz - pHeader->filesz);
        }
        
        break;
      
      case PT_DYNAMIC:
        printf("[relocModule_elf] Program header #%d Type:DYNAMIC Addr:%h Size:%h!\n", i, pHeader->vaddr, pHeader->filesz);
        //I failed to understand what needs to be done here
        break;
  
      case PT_INTERP:
        printf("[relocModule_elf] Program header #%d Type:INTERP Addr:%h Size:%h!\n", i, pHeader->vaddr, pHeader->filesz);
        
        //This is an external program that needs to be relocated and called.
        static unsigned char *interp;
        
        memcpy((char*)interp, (char*)offset + pHeader->offset, pHeader->filesz);
        
        printf("\tInterpreter: \"%s\"\n", interp);
        break;
      
      case PT_NOTE:
        printf("[relocModule_elf] Program header #%d Type:NOTE Addr:%h Size:%h!\n", i, pHeader->vaddr, pHeader->filesz);
      
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


u32int openModule_elf(char *name, u32int offset)
{
  printf("[openModule_elf] Opening %s (ELF module)\n",name);
  
  Elf32_Ehdr *header;
  
  /* Put header information into header struct */
  header = (Elf32_Ehdr*)offset;
  
  /* Check for any errors */
  if(header->ident[0] != 0x7f || header->ident[1] != 'E' || header->ident[2] != 'L' || header->ident[3] != 'F')
  {
    printf("[openModule_elf] Error: not a valid ELF binary\n");
    return 1;
  }
  
  /* Bits */
  if(header->ident[4] != 1) //1 = 32, 2 = 64
  { 
    printf("[openModule_elf] Error: wrong architecture bit number\n");
    return 1;
  }
  
  /* Endian type */
  if(header->ident[5] != 1) //1 = little, 2 = big
  {
    printf("[openModule_elf] Error: wrong architecture endianess\n");
    return 1;
  }
  
  //~ debug_sectionHeaders(header);
  
  if(relocModule_elf(header, offset) != 1)
  {
    printf("[openModule_elf] Error: relocation failed\n");
    return 1;
  }
  
  printf("[openModule_elf] Calling executable at %h\n", header->entry);
  
  /* Call the program */
  for(;;);
  //~ asm volatile("call *%0" : : "a" (offset));
  asm volatile("call *%0" : : "a" (header->entry));
  //~ asm volatile("call *%0" : : "a" (header));
  //~ execute_binary(header->entry);
  //~ start_task(PRIO_LOW, PROC_SHORT, (void*)header->entry, 0, "try");
  return 0;
}

/*-----------------------------------------------------------------------------
 *
 *---------------------------------------------------------------------------*/

elf_sections_t* load_elf ( char* name ) {
	int i = 0;
	struct dirent *node = 0;
	fs_node_t *elf_node = 0;

	while ( ( node = readdir_fs ( fs_root, i ) ) != 0 ) {
		//printf ( "Found file %s\n" , node->name );
		fs_node_t *fsnode = finddir_fs ( fs_root, node->name );

		if ( checkstr ( node->name, name ) ) {
			printf ( "[ELF] Found %s\n", node->name );
			elf_node = fsnode;
		}

		i++;
	}

	u32int inode = elf_node->inode;
	u32int size = elf_node->length;
	printf ( "[ELF] Called for inode %d - %d bytes of data allocated\n", inode, size );
	char *elf_name = fs_root[inode + 2].name; // plus two because initrd and dev take up two inodes for some reason
	printf ( "[ELF] %s == %s\n", name, elf_name );

	if ( !checkstr ( name, elf_name ) ) {
		printf ( "[ELF] inode file not correct file\n" );
		return 0;
	}

	//printf("[ELF] ");
	FILE *modulenode;

	//tests if this module exist in the root
	if(!(modulenode = finddir_fs(fs_root, elf_name)))
		return (elf_sections_t*)0;
	else{ //the module exists
		printf("[ELF] Directory entry read correctly\n");
		printf("[ELF] Looking for \"%s\"\n", elf_name);
		
		printf("[ELF] Reading file\n");
		//create the actuall module buffer 
		char *modulebuffer = (char*)kmalloc(size);
    
		//open the file
		u32int modulesize = read_fs(modulenode, 0, size, modulebuffer);
		printf("[ELF] size: %d\n", modulesize);
		
		printf("[ELF] Allocating space\n");

		//create a pointer and copy the above saved buffer that address
		u32int moduleptr = kmalloc(modulesize);
		memcpy((void*)moduleptr, (void*)modulebuffer, modulesize);
		
		printf("[ELF] Data moved into allocated space: %h\n", moduleptr);
		
		if(openModule_elf(elf_name, moduleptr))
		printf("[load_elf] openModule_elf returned with an error\n");
    
		//free stuff
		kfree(modulebuffer);
		kfree((u32int*)moduleptr);
		//TODO f_finddir_close(modulenode);
	}
}

