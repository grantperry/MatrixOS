#ifndef STDIO_H
#define STDIO_H

#include "fs.h"
#include "vfs.h"

/*
// std print function takes multiple arguments and prints them.
*/
void printf ( char *c, ... );

/* Open file */
FILE* fopen ( fs_node_t* dir, char* file_name, char* mode );

/* Find file in directory */
FILE* find ( fs_node_t* dir, char* file_name );

/* Read data from file */
u32int fread ( FILE* file, void* buf, u32int size ); /* Set pointer to required position into file */

void fseek ( FILE* file, u32int seek_pos );

#endif //STDIO_H
