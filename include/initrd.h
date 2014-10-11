// MatrixOS - initrd.h
// Defines the interface for and structures relating to the initial ramdisk.

#ifndef INITRD_H
#define INITRD_H

#include "common.h"
#include "vfs.h"
#include "fs.h"

typedef struct fs_node fs_node_t;

typedef struct {
	u32int nfiles; // The number of files in the ramdisk.
} initrd_header_t;

typedef struct {
	u8int magic;     // Magic number, for error checking.
	s8int name[64];  // Filename.
	u32int offset;   // Offset in the initrd that the file starts.
	u32int length;   // Length of the file.
} initrd_file_header_t;

initrd_header_t *initrd_header;	 // The header.
initrd_file_header_t *file_headers; // The list of file headers.
fs_node_t *initrd_root;			 // Our root directory node.
fs_node_t *initrd_dev;			  // We also add a directory node for /dev, so we can mount devfs later on.
fs_node_t *root_nodes;			  // List of file nodes.
int nroot_nodes;					// Number of file nodes.

// Initialises the initial ramdisk. It gets passed the address of the multiboot module,
// and returns a completed filesystem node.
fs_node_t *initialise_initrd ( u32int location );

u32int initrd_read ( fs_node_t *node, u32int offset, u32int size, u8int *buffer );

#endif
