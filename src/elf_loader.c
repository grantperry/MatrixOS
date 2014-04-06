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
  //static notee_entry* note;
  
  printf("[relocModule_elf] %d Program Headers found, e_phentsize %d.\n", header->e_phnum, header->e_phentsize);
  
  for(i = 0; i < header->e_phnum; i++)
  {
    pHeader = (Elf32_Phdr*)((unsigned long) header + header->e_phoff + i * header->e_phentsize);
    
    /* Check if its loadable */
    switch(pHeader->p_type)
    {
     
      case PT_LOAD:
        printf("[relocModule_elf] Program header #%d e_type:LOAD Offset:%h Vaddr:%h, Paddr:%h, Size:%h, MEM:%h, Align:%h!\n", i, pHeader->p_offset, pHeader->p_vaddr, pHeader->p_paddr, pHeader->p_fileSize, pHeader->p_memSize, pHeader->p_alignment);

        //~ page_t *page;
//~ 
        //~ u32int j = pHeader->vaddr & ~0xFFF;
        //~ u32int limit = ((u32int) pHeader->vaddr + pHeader->memSize + 0x1000) & ~0xFFF;

        //Map the virtual address to an area in the stack
        //~ for(;j < limit; j+= 0x1000)
        //~ {
          //~ page = get_page(pHeader->vaddr, 1, current_directory);
          //~ alloc_frame(page, 0, 1);
          //~ k_printf("frame:  %h\n", page->frame);
        //~ }
        
        //Map the virtual address to an area in the stack
        virtual_map_pages(pHeader->p_vaddr, pHeader->p_fileSize, TRUE, TRUE);

        printf("[relocModule_elf] pHeader->vaddr source: %h\n", offset + pHeader->p_offset);

        //~ memcpy((u32int*)pHeader->vaddr, (const void*)offset + pHeader->offset, pHeader->memSize);
        
        ///* Copy the program data to vaddr */
        memcpy((char*)pHeader->p_vaddr, (char*)offset + pHeader->p_offset, pHeader->p_fileSize);
        //~ memcpy((char*)pHeader->vaddr, (char*)offset + pHeader->offset, pHeader->memSize);
        if(pHeader->p_fileSize < pHeader->p_memSize)
        {
          /* memset() the remaining memory with zeroes - stack */
          memset((char*)pHeader->p_vaddr + pHeader->p_fileSize, 0, pHeader->p_memSize - pHeader->p_fileSize);
        }
        
        break;
      
      case PT_DYNAMIC:
        printf("[relocModule_elf] Program header #%d e_type:DYNAMIC Addr:%h Size:%h!\n", i, pHeader->p_vaddr, pHeader->p_fileSize);
        //I failed to understand what needs to be done here
        break;
  
      case PT_INTERP:
        printf("[relocModule_elf] Program header #%d e_type:INTERP Addr:%h Size:%h!\n", i, pHeader->p_vaddr, pHeader->p_fileSize);
        
        //This is an external program that needs to be relocated and called.
        static unsigned char *interp;
        
        memcpy((char*)interp, (char*)offset + pHeader->p_offset, pHeader->p_fileSize);
        
        printf("\tInterpreter: \"%s\"\n", interp);
        break;
      
      case PT_NOTE:
        printf("[relocModule_elf] Program header #%d e_type:NOTE Addr:%h Size:%h!\n", i, pHeader->p_vaddr, pHeader->p_fileSize);
      
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

/*-----------------------------------------------------------------------------
 *
 *---------------------------------------------------------------------------*/
elf_sections_t* load_elf ( char* name, u32int offset) {
	printf("[openModule_elf] Opening %s (ELF module)\n",name);
  
  Elf32_Ehdr *header;
  
  /* Put header information into header struct */
  header = (Elf32_Ehdr*)offset;
  
  /* Check for any errors */
  if(header->e_ident[0] != 0x7f || header->e_ident[1] != 'E' || header->e_ident[2] != 'L' || header->e_ident[3] != 'F')
  {
    printf("[openModule_elf] Error: not a valid ELF binary\n");
    return (elf_sections_t*)1;
  }
  
  /* Bits */
  if(header->e_ident[4] != 1) //1 = 32, 2 = 64
  { 
    printf("[openModule_elf] Error: wrong architecture bit number\n");
    return (elf_sections_t*)1;
  }
  
  /* Endian e_type */
  if(header->e_ident[5] != 1) //1 = little, 2 = big
  {
    printf("[openModule_elf] Error: wrong architecture endianess\n");
    return (elf_sections_t*)1;
  }
  
  //~ debug_sectionHeaders(header);
  
  if(relocModule_elf(header, offset) != 1)
  {
    printf("[openModule_elf] Error: relocation failed\n");
    return (elf_sections_t*)1;
  }
  
  printf("[openModule_elf] Calling executable at %h\n", header->e_entry);
  
  /* Call the program */
  for(;;);
  //~ asm volatile("call *%0" : : "a" (offset));
  asm volatile("call *%0" : : "a" (header->e_entry));
  //~ asm volatile("call *%0" : : "a" (header));
  //~ execute_binary(header->e_entry);
  //~ start_task(PRIO_LOW, PROC_SHORT, (void*)header->e_entry, 0, "try");
  
  return 0;
}
