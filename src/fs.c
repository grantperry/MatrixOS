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

u32int f_read(FILE file, u32int offset, u32int size, u8int *buffer) {

}
