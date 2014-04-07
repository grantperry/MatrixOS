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
	int i = 0;
	struct dirent *node = 0;
	fs_node_t *elf_node = 0;
	while ( ( node = readdir_fs ( fs_root, i ) ) != 0 ) {
		//printf ( "Found file %s\n" , node->name );
		fs_node_t *fsnode = finddir_fs ( fs_root, node->name );
		if(checkstr(node->name, name)) {
			printf("[ELF] Found %s\n", node->name);
			elf_node = fsnode;
		}
		i++;
	}
	u32int inode = elf_node->inode;
	u32int size = elf_node->length;
	printf("[ELF] Called for inode %d - %d bytes of data allocated\n", inode, size);
	char *elf_name = fs_root[inode + 2].name; // plus two because initrd and dev take up two inodes for some reason
	printf("[ELF] %s == %s\n", name, elf_name);
	if(!checkstr(name, elf_name)) {
		printf("[ELF] inode file not correct file\n");
		return 0;
	}
	//printf("[ELF] ");
	FILE *modulenode;
	//tests if this module exist in the root
	if(!(modulenode = finddir_fs(fs_root, elf_name)))
		return (elf_sections_t*)0;
	else{ //the module exists
		printf("[load_elf] Directory entry read correctly\n");
		printf("[load_elf] Looking for \"%s\"\n", elf_name);
		
		printf("[load_elf] Reading file\n");
		//create the actuall module buffer 
		char *modulebuffer = (char*)kmalloc(size);
    
		//open the file
		FILE *modulenode_file;
		//modulenode_file = f_open(modulenode->name, fs_root, "r");
		
		
	}
	
}
