#include "fs.h"

//the curent directory node
void *currentDirectory;

//initial file descriptor location
file_desc_t *initial_file_desc;

/*
// Checks weather the file is already open or not.
*/
file_desc_t *lookup_file_desc(void *node) {
	file_desc_t *temp_desc;
	temp_desc = initial_file_desc; //make a copy so we dont muck it up.
	
	for(; temp_desc != node && temp_desc; temp_desc = temp_desc->next) {}
	
	if(!temp_desc) {
		return 0; //descriptor is null, return 0.
	}
	return temp_desc;
}

u32int f_read(file_desc_t *file, u32int offset, u32int size, u8int *buffer) {
	file_desc_t *fdesc;
	fdesc = (file_desc_t*) lookup_file_desc(file->node);

	//we did not find the file desc in the list
	if(!fdesc)
	{
		printf("ERROR: file not in file descriptor list\n");
		return 0; //error
	}
	if(!(fdesc->permisions & FDESC_READ)) {
		printf("ERROR: you dont have permisions to read that!\n");
		return 0;
	}
	
	switch(fdesc->fs_type) {
		case M_UNKNOWN:
			return 0; //error
		case M_VFS:
			//check if this node has a callback
			if(((fs_node_t*)fdesc->node)->read) {
				return ((fs_node_t*)fdesc->node)->read(fdesc->node, offset, size, buffer);
			} else {
				break;
			}
		case M_EXT2:
			//return ext2_read(fdesc->node, offset, size, buffer);
			printf("TODO Support ext2-4\n");
			break;
		default:
			return 0; //error
    }
}
