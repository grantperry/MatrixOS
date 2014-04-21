#include "vfs.h"
#include "initrd.h"

fs_node_t *fs_root = 0; // The root of the filesystem.

extern fs_node_t *initrd_root;             // Parent of root
extern fs_node_t *initrd_dev;              // Root dir
extern fs_node_t *root_nodes;              // List of file nodes.
extern int nroot_nodes;                    // Number of file nodes.

/*
// Read a section of data from node
// TODO make shure this makes sense...
*/
u32int read_fs ( fs_node_t *node, u32int offset, u32int size, u8int *buffer ) {
	// Has the node got a read callback?
	if ( node->read != 0 ) {
		return node->read ( node, offset, size, buffer );

	} else {
		return 0;
	}
}

/*
// Write a section of data to node
// TODO make shure this makes sense...
*/
u32int write_fs ( fs_node_t *node, u32int offset, u32int size, u8int *buffer ) {
	// Has the node got a write callback?
	if ( node->write != 0 ) {
		return node->write ( node, offset, size, buffer );

	} else {
		return 0;
	}
}

/*
// open a filesystem for reading.
// TODO make shure this makes sense...
*/
void open_fs ( fs_node_t *node, u8int read, u8int write ) {
	// Has the node got an open callback?
	if ( node->open != 0 ) {
		return node->open ( node );
	}
}

/*
// Close a filesystem.
// TODO make shure this makes sense...
*/
void close_fs ( fs_node_t *node ) {
	// Has the node got a close callback?
	if ( node->close != 0 ) {
		return node->close ( node );
	}
}

/*
// TODO figure this out.
*/
struct dirent *readdir_fs ( fs_node_t *node, u32int index ) {
	// Is the node a directory, and does it have a callback?
	if ( ( node->flags&0x7 ) == FS_DIRECTORY &&
			node->readdir != 0 ) {
		return node->readdir ( node, index );

	} else {
		return 0;
	}
}

/*
// TODO figure this out
*/
fs_node_t *finddir_fs ( fs_node_t *node, char *name ) {
	// Is the node a directory, and does it have a callback?
	if ( ( node->flags&0x7 ) == FS_DIRECTORY &&
			node->finddir != 0 ) {
		return node->finddir ( node, name );

	} else {
		return 0;
	}
}

