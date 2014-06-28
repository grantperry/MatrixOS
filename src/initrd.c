// initrd.c -- Defines the interface for and structures relating to the initial ramdisk.

#include "initrd.h"

initrd_header_t *initrd_header;	 // The header.
initrd_file_header_t *file_headers; // The list of file headers.

extern fs_node_t *initrd_root;             // Parent of root
extern fs_node_t *initrd_dev;              // Root dir
extern fs_node_t *root_nodes;              // List of file nodes.
extern int nroot_nodes;                    // Number of file nodes.

struct dirent dirent;

/*
// place the data specified in the memory specified.
*/
u32int initrd_read ( fs_node_t *node, u32int offset, u32int size, u8int *buffer ) {
	initrd_file_header_t header = file_headers[node->inode];

	if ( offset > header.length ) {
		return 0;
	}

	if ( offset+size > header.length ) {
		size = header.length-offset;
	}

	memcpy ( buffer, ( u8int* ) ( header.offset+offset ), size );
	return size;
}

/*
// Read a directory of the initrd file
*/
struct dirent *initrd_readdir ( fs_node_t *node, u32int index ) {
	if ( node == initrd_root && index == 0 ) {
		strcpy ( dirent.name, "dev" );
		dirent.name[3] = 0;
		dirent.ino = 0;
		return &dirent;
	}

	if ( index-1 >= nroot_nodes ) {
		return 0;
	}

	strcpy ( dirent.name, root_nodes[index-1].name );
	dirent.name[strlen ( root_nodes[index-1].name )] = 0;
	dirent.ino = root_nodes[index-1].inode;
	return &dirent;
}

/*
// Find a directory and return the pointer.
*/
fs_node_t *initrd_finddir ( fs_node_t *node, char *name ) {
	if ( node == initrd_root && !strcmp ( name, "dev" ) ) {
		return initrd_dev;
	}

	int i;

	for ( i = 0; i < nroot_nodes; i++ )
		if ( !strcmp ( name, root_nodes[i].name ) ) {
			serialf ( "[INITRD][FINDDIR]  inode: %d\tint_inode: %d\tname: \"%s\"\n", i, root_nodes[i].inode, root_nodes[i].name );
			return &root_nodes[i];
		}

	return 0;
}

/*
// Initalise the InitRD.
*/
fs_node_t *initialise_initrd ( u32int location ) {
	// Initialise the main and file header pointers and populate the root directory.
	initrd_header = ( initrd_header_t * ) location;
	file_headers = ( initrd_file_header_t * ) ( location+sizeof ( initrd_header_t ) );

	// Initialise the root directory.
	initrd_root = ( fs_node_t* ) kmalloc ( sizeof ( fs_node_t ) );
	strcpy ( initrd_root->name, "initrd" );
	strcpy ( initrd_root->name + sizeof ( "initrd" ), '\000' );
	initrd_root->mask = initrd_root->uid = initrd_root->gid = initrd_root->inode = initrd_root->length = 0;
	initrd_root->fstype = M_VFS;
	initrd_root->magic = NODE_DIRECTORY;
	initrd_root->read = 0;
	initrd_root->write = 0;
	initrd_root->open = 0;
	initrd_root->close = 0;
	initrd_root->readdir = &initrd_readdir;
	initrd_root->finddir = &initrd_finddir;
	initrd_root->ptr = 0;
	initrd_root->impl = 0;

	// Initialise the /dev directory (required!)
	initrd_dev = ( fs_node_t* ) kmalloc ( sizeof ( fs_node_t ) );
	strcpy ( initrd_dev->name, "dev" );
	initrd_dev->mask = initrd_dev->uid = initrd_dev->gid = initrd_dev->inode = initrd_dev->length = 0;
	initrd_dev->fstype = M_VFS;
	initrd_dev->magic = NODE_DIRECTORY;
	initrd_dev->read = 0;
	initrd_dev->write = 0;
	initrd_dev->open = 0;
	initrd_dev->close = 0;
	initrd_dev->readdir = &initrd_readdir;
	initrd_dev->finddir = &initrd_finddir;
	initrd_dev->ptr = 0;
	initrd_dev->impl = 0;

	root_nodes = ( fs_node_t* ) kmalloc ( sizeof ( fs_node_t ) * initrd_header->nfiles * sizeof ( fs_node_t ) ); //8 extra files////////////////////////////////////////////////////////////////////
	nroot_nodes = initrd_header->nfiles;

	// For every file...
	int i;

	for ( i = 0; i < initrd_header->nfiles; i++ ) {
		// Edit the file's header - currently it holds the file offset
		// relative to the start of the ramdisk. We want it relative to the start
		// of memory.
		file_headers[i].offset += location;
		// Create a new file node.
		strcpy ( root_nodes[i].name, &file_headers[i].name );
		root_nodes[i].mask = root_nodes[i].uid = root_nodes[i].gid = 0;
		root_nodes[i].length = file_headers[i].length;
		root_nodes[i].inode = i;
		root_nodes[i].fstype = M_VFS;
		root_nodes[i].magic = NODE_FILE;
		root_nodes[i].read = &initrd_read;
		root_nodes[i].write = 0;
		root_nodes[i].readdir = 0;
		root_nodes[i].finddir = 0;
		root_nodes[i].open = 0;
		root_nodes[i].close = 0;
		root_nodes[i].impl = 0;

		serialf ( "[INITRD][REGISTER] name: %s,\tlength: %d\tinode: %d\tmagic: %d\n", file_headers[i].name, file_headers[i].length, i, root_nodes[i].magic );
	}


	return initrd_root;
}
