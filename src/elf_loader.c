#include	"elf_loader.h"
#include 	"fs.h"

extern fs_node_t* fs_root;

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

		//if ( openModule_elf ( modulenode->name, moduleptr ) ) {
		//	printf ( "[load_elf] openModule_elf returned with an error\n" );
		//}

		//free stuff
		kfree ( modulebuffer );
		kfree ( ( u32int* ) moduleptr );
		//TODO f_finddir_close(modulenode);
	}
	

}
