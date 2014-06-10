#include "vfs.h"
#include "fs.h"

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

fs_node_t *vfs_createFile ( fs_node_t *parentNode, char *name, u32int size ) {
	u32int n = findOpenNode();
	printf ( "" );

	// Create a new file node.
	strcpy ( root_nodes[n].name, name );
	root_nodes[n].node_type = M_VFS;
	root_nodes[n].mask = 0b110110100; //user rw, group rw, other r
	root_nodes[n].uid = root_nodes[n].gid = 0;
	root_nodes[n].inode = n;
	root_nodes[n].length = size;

	//set the pointers to singly, doubly, triply to 0
	root_nodes[n].pointer = 0;

	root_nodes[n].flags = FS_FILE;
	root_nodes[n].read = &initrd_read;
	//~ root_nodes[n].write = 0;
	//root_nodes[n].write = &initrd_write;
	root_nodes[n].readdir = 0;
	root_nodes[n].finddir = 0;
	root_nodes[n].impl = 0;

	printf ( "1\n" );

	root_nodes[n].pointer = ( fs_block_t* ) kmalloc ( sizeof ( fs_block_t ) ); //pointer to first block node.
	root_nodes[n].pointer->length = size;// size of the block pointed to.
	root_nodes[n].pointer->pointer = kmalloc ( size*sizeof ( char ) ); //the actuall pointer to the block
	memset ( root_nodes[n].pointer->pointer, 0x0, size*sizeof ( char ) ); //set the block area to 0's
	root_nodes[n].pointer->next = 0;//pointer to the next block in the file.

	printf ( "2\n" );
	addFileToDir ( parentNode, &root_nodes[n] );
	printf ( "6\n" );
	return &root_nodes[n];

}

int findOpenNode() {
	int i;

	for ( i = 0; i < nroot_nodes; i++ )

		//if all of those headers are not used, then there is no file
		if ( !* ( root_nodes[i].name ) && !root_nodes[i].inode && !root_nodes[i].mask && !root_nodes[i].flags ) {
			return i;
		}

	nroot_nodes++;
	return nroot_nodes - 1;
}

int addFileToDir ( fs_node_t *dirNode, fs_node_t *fileNode ) {
	u32int fileINode = fileNode->inode;

	struct dirent dirent;

	u32int lengthOfName = strlen ( fileNode->name );
	dirent.ino = fileINode;

	dirent.file_type = fileNode->node_type;

	//The size of this whole struct basically
	dirent.rec_len = sizeof ( dirent.ino ) + sizeof ( dirent.rec_len ) + sizeof ( dirent.name ) + sizeof ( dirent.file_type ) + sizeof ( dirent.name_len ) + lengthOfName;

	dirent.name_len = ( u8int ) lengthOfName;
	dirent.file_type = fileNode->flags;
	printf ( "3\n" );

	//+1 being the \000 NULL termination 0 byte at the end of the string
	dirent.name = ( char* ) kmalloc ( lengthOfName + 1 );

	//copy the name of the file (fileNode->name) to the dirent.name
	//TODO make fs_node_t use dynamic name sizes
	memcpy ( dirent.name, fileNode->name, lengthOfName + 1 ); //fileNode->name already had a \000 termination zero
	printf ( "4\n" );
	* ( dirent.name + lengthOfName ) = 0; //Just in case add a \000 at the end

	dirNode->dpointer = ( struct dirent* ) kmalloc ( sizeof ( dirent ) );
	memcpy ( dirNode->dpointer, &dirent, sizeof ( dirent ) );

	strcpy ( ( char* ) ( * ( u32int* ) dirNode->dpointer + sizeof ( dirent.ino ) + sizeof ( dirent.rec_len )
						 + sizeof ( dirent.name_len ) + sizeof ( dirent.file_type ) ), dirent.name );

	kfree ( dirent.name );

	//success!
	return 0;
}

//This may be difficult to understand due to lack of explaining comments
u32int *block_of_set ( fs_node_t *node, u32int block_number ) {
	return 0;
}
