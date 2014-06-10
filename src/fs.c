#include "fs.h"
#include "vfs.h"

//the curent directory node
void *currentDirectory;

//initial file descriptor location
file_desc_t *initial_file_desc;

/*
// Checks weather the file is already open or not.
*/
file_desc_t *lookup_file_desc ( void *node ) {
	file_desc_t *temp_desc;
	temp_desc = initial_file_desc; //make a copy so we dont muck it up.

	for ( ; temp_desc != node && temp_desc; temp_desc = temp_desc->next ) {}

	if ( !temp_desc ) {
		return 0; //descriptor is null, return 0.
	}

	return temp_desc;
}

//TODO actually read the file
u32int f_read(file_desc_t *file, u32int offset, u32int size, u8int *buffer) {
	file_desc_t *fdesc;
	fdesc = ( file_desc_t* ) lookup_file_desc ( file->node );

	//we did not find the file desc in the list
	if ( !fdesc ) {
		printf ( "ERROR: file not in file descriptor list\n" );
		return 0; //error
	}

	if ( ! ( fdesc->permisions & FDESC_READ ) ) {
		printf ( "ERROR: you dont have permisions to read that!\n" );
		return 0;
	}

	switch ( fdesc->fs_type ) {
	case M_UNKNOWN:
		return 0; //error

	case M_VFS:

		//check if this node has a callback
		if ( ( ( fs_node_t* ) fdesc->node )->read ) {
			return ( ( fs_node_t* ) fdesc->node )->read ( fdesc->node, offset, size, buffer );

		} else {
			break;
		}

	case M_EXT2:
		//return ext2_read(fdesc->node, offset, size, buffer);
		printf ( "TODO Support ext2-4\n" );
		break;

	default:
		return 0; //error
	}
}

u32int f_write ( FILE *node, u32int offset, u32int size, u8int *buffer ) {
	//check if this FILE node is in the file descriptor list
	file_desc_t *fdesc;
	fdesc = lookup_file_desc ( node->node );

	//we did not find the file desc in the list
	if ( !fdesc ) {
		printf ( "Error: file not in file descriptor list\n" );
		return 1; //error
	}

	//if the size writing to the node is larger than its size + offset, expand it
	if ( offset + size > node->size )

		//TODO f_expand(node, (offset + size) - node->size);

		//if the user can read from it
		if ( fdesc->permisions & FDESC_WRITE ) {
			switch ( fdesc->fs_type ) {
			case M_UNKNOWN:
				return 1; //error

			case M_VFS:

				//check if this node has a callback
				if ( ( ( fs_node_t* ) fdesc->node )->write ) {
					return ( ( fs_node_t* ) fdesc->node )->write ( fdesc->node, offset, size, buffer );

				} else {
					break;
				}

			/*case M_EXT2:
			  return ext2_write(fdesc->node, offset, size, buffer);*/
			default:
				return 1; //error
			}

		} else {
			printf ( "Error: writing to an unprivilaged file\n" );
		}

	//if we have not exited yet, it is an error
	return 1;
}

static u8int __open_fs_mask_to_u32int__ ( char *mask ) {
	if ( !mask ) {
		return 0;    //then there is no mask to return
	}

	u8int flags = 0;
	u32int i;

	for ( i = 0; i < strlen ( mask ); i++ ) {
		//assign the values of the flags
		switch ( * ( mask + i ) ) {
		case 'd':
			flags |= FDESC_CLEAR;
			break;

		case 'r':
			flags |= FDESC_READ;
			break;

		case 'w':
			flags |= FDESC_WRITE;
			break;

		case 'a':
			flags |= FDESC_APPEND;
			break;
		}
	}

	return flags;
}

//TODO does not work, causes page fault in initialize vfs
u32int node_type ( void *node ) {
	switch ( ( ( FILE* ) node )->node_type ) {
	case M_UNKNOWN:
		return nTYPE_UNKOWN; //error

	case M_VFS:
		switch ( ( ( fs_node_t* ) node )->flags ) {
		case FS_FILE:
			return nTYPE_FILE;

		case FS_DIRECTORY:
			return nTYPE_DIRECTORY;

		case FS_CHARDEVICE:
			return nTYPE_CHARD_DEV;

		case FS_BLOCKDEVICE:
			return nTYPE_BLOCK_DEV;

		case FS_PIPE:
			return nTYPE_FIFO;

		case FS_SYMLINK:
			return nTYPE_SYMLINK;

		case FS_MOUNTPOINT:
			return nTYPE_MOUNTPOINT;

		default:
			return nTYPE_UNKOWN;
		}

	/*case M_EXT2:
	  switch(((ext2_inode_t*)node)->type)
	  {
	  case EXT2_FILE:
	    return TYPE_FILE;
	  case EXT2_DIR:
	    return TYPE_DIRECTORY;
	  case EXT2_CHARD_DEV:
	    return TYPE_CHARD_DEV;
	  case EXT2_BLOCK_SZ:
	    return TYPE_BLOCK_DEV;
	  case EXT2_FIFO:
	    return TYPE_FIFO;
	  case EXT2_SYMLINK:
	    return TYPE_SYMLINK;
	  case EXT2_MOUNTPOINT:
	    return TYPE_MOUNTPOINT;
	  default:
	    return TYPE_UNKOWN;
	  }*/
	default:
		return nTYPE_UNKOWN; //error

	}

	//if we are outside, that is an error
	return nTYPE_UNKOWN;
}

FILE *__open__ ( void *node, char *name, char *mask, u8int open ) {
	if ( node ) {
		file_desc_t *tmp_desc, *new_desc;
		tmp_desc = initial_file_desc;

		//a simple error check if the tmp_desc exists
		if ( !tmp_desc ) {
			return 0;
		}

		/*go to the end of out file descriptor list
		 * while iterating, check if this file_desc already exists,
		 * if true, return an error*/
		for ( ; tmp_desc->next; tmp_desc = tmp_desc->next )

			//if we already have this file node in the list
			if ( tmp_desc->node == node ) {
				return tmp_desc;    //no need to open, just return it
			}

		//make the new list entry and add it to the end of the list
		new_desc = ( file_desc_t* ) kmalloc ( sizeof ( file_desc_t ) );

		//copy the name over
		u32int name_len = strlen ( name );
		new_desc->name = ( char* ) kmalloc ( name_len + 1 ); // +1 for the \000
		memcpy ( new_desc->name, name, name_len + 1 );
		new_desc->name_length = name_len;

		new_desc->node = node;
		//TODO new_desc->fs_type = ((generic_fs_t*)node)->magic;
		new_desc->node_type = node_type ( node );
		new_desc->permisions = __open_fs_mask_to_u32int__ ( mask );

		//set some file system specific elements
		switch ( new_desc->fs_type ) {
		case M_UNKNOWN:
			kfree ( new_desc->name );
			kfree ( new_desc );
			return 0; //error

		case M_VFS:
			new_desc->inode = ( ( fs_node_t* ) node )->inode;
			new_desc->size = ( ( fs_node_t* ) node )->length;

			//if it is a directory
			if ( new_desc->node_type == nTYPE_DIRECTORY ) {
				new_desc->_read = 0;
				new_desc->_write = 0;
				new_desc->_finddir = ( void* ) finddir_fs;
				new_desc->_readdir = ( void* ) readdir_fs;

			} else {
				new_desc->_read = ( void* ) read_fs;
				new_desc->_write = ( void* ) write_fs;
				new_desc->_finddir = 0;
				new_desc->_readdir = 0;
			}

			break;

		/*case M_EXT2:
		  new_desc->inode = ((ext2_inode_t*)node)->inode;
		  new_desc->size = ((ext2_inode_t*)node)->size;

		  //if it is a directory
		  if(new_desc->node_type == TYPE_DIRECTORY)
		  {
		    new_desc->_read = 0;
		    new_desc->_write = 0;
		    new_desc->_finddir = (void*)ext2_file_from_dir;
		    new_desc->_readdir = (void*)ext2_dirent_from_dir;
		  }else{
		    new_desc->_read = (void*)ext2_read;
		    new_desc->_write = (void*)ext2_write;
		    new_desc->_finddir = 0;
		    new_desc->_readdir = 0;
		  }

		  break;*/
		default:
			kfree ( new_desc->name );
			kfree ( new_desc );
			return 0; //error
		}

		new_desc->next = 0;

		//add this file descriptor to the overall list, only if the user wants to
		if ( open == TRUE ) {
			tmp_desc->next = new_desc;
		}

		return new_desc;

	} else {
		return 0;
	}
}

FILE *f_open(char *filename, void *dir, char *mask)
{
  if(!dir)
    return 0; //error

  FILE *file;
  file = (FILE *)f_finddir(dir, filename);

  //a file already exists to be opened
  if(file)
  {
	}

  //if we are outside, return an error
  return 0;

}

FILE *f_finddir ( void *node, char *name ) {
	//is this node a directory
	if ( node_type ( node ) == nTYPE_DIRECTORY ) {
		//if name is input 0, then we should find the name of the node
		if ( !name ) {
			name = name_of_dir ( node );
		}

		switch ( ( ( FILE* ) node )->node_type ) {
		case M_UNKNOWN:
			return 0; //error

		case M_VFS: {
				//case the void * node to the vfs node structure
				fs_node_t *vfs_node = node;

				//if the node has a callback?
				if ( vfs_node->finddir )
					//return an unopened file node with no r/w/a permissions at all to the actuall node data
				{
					return __open__ ( vfs_node->finddir ( vfs_node, name ), name, 0, FALSE );

				} else {
					break;
				}
			}

		/*case M_EXT2:
		  //return an unopened file node with no r/w/a permissions at all to the actuall node data
		  return __open__(ext2_file_from_dir(node, name), name, 0, FALSE);*/
		default:
			return 0; //error

		}

	}

	//if we are outside, regard it as an error
	return 0; //error
}

char *name_of_dir ( void *node ) {
	if ( node_type ( node ) == nTYPE_DIRECTORY ) {
		switch ( ( ( FILE* ) node )->node_type ) {
		case M_UNKNOWN:
			return 0; //error

		case M_VFS:
			return ( ( fs_node_t* ) node )->name;

		/*case M_EXT2:
		  return ext2_get_name_of_dir(node);*/
		default:
			return 0; //error
		}

	} else {
		return 0;    //error
	}
}
