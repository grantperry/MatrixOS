// MatrixOS - fs.c
//
#include <fs.h>
#include <vfs.h>

//the curent directory node
void *currentDirectory;

// The Verbosity of the f_* commands
u8int f_verbose;

/* Initial file descriptor location
// This holds the descriptors for open files.
*/
file_desc_t *initial_file_desc;


/* set the Verbosity of the f_* commands
// 0 = no output
// 1 = file names
// 2 = everything
*/
u8int set_f_verbose ( u8int vb ) {
	f_verbose = vb;
	return f_verbose;
}

s8int init_file_system() {
	printf ( "Initalizing File System." );
	set_f_verbose ( VB_NONE );
	initial_file_desc = ( file_desc_t* ) kmalloc ( sizeof ( file_desc_t ) );
	strcpy ( initial_file_desc->name, "root" );
	initial_file_desc->inode = -1;
	initial_file_desc->next = 0;
	// We need to load initrd files to the init_file_desc.

	return 0;
}

/*
// Checks weather the file is already open or not.
*/
file_desc_t *lookup_file_desc ( void *node ) {
	file_desc_t *tmp_desc;
	tmp_desc = initial_file_desc;

	for ( ; tmp_desc; tmp_desc = tmp_desc->next ) {
		if ( tmp_desc ) {
			if ( f_verbose >= VB_ALL ) {
				serialf ( "[FDESC][LOOKUP] %s", tmp_desc->name );
			}

			if ( tmp_desc == node ) {
				if ( f_verbose >= VB_ALL ) {
					serialf ( " FOUND\n" );
				}

				return ( file_desc_t* ) tmp_desc;
			}

			serialf ( "\n" );
		}
	}

	return 0;
}

//TODO actually read the file
u32int f_read ( file_desc_t *file, u32int offset, u32int size, u8int *buffer ) {
	file_desc_t *fdesc;
	fdesc = ( file_desc_t* ) lookup_file_desc ( file ); //search for the

	// descriptor in the list.
	if ( !fdesc ) {
		if ( f_verbose >= VB_ALL ) {
			serialf ( "[FS][READ] File not Open!\n" );
		}

		return 0;
	}

	if ( fdesc->permisions & FDESC_READ ) {
		switch ( fdesc->fs_type ) {
		case M_UNKNOWN:
			return 1; //error

		case M_VFS:

			//check if this node has a callback
			if ( ( ( fs_node_t* ) fdesc->node )->read ) {
				return ( ( fs_node_t* ) fdesc->node )->read ( fdesc->node, offset, size, buffer );

			} else {
				break;
			}

		default:
			return 1; //error
		}

	} else {
		if ( f_verbose >= VB_ALL ) {
			serialf ( "[FS][READ][ERROR] You dont have read permisions\n" );
		}

		return 0;
	}

	if ( f_verbose >= VB_ALL ) {
		serialf ( "[FS] File is not linked to a Filesystem" );
	}

	return 0; //error
}

u32int f_write ( FILE *node, u32int offset, u32int size, u8int *buffer ) {
	//check if this FILE node is in the file descriptor list
	file_desc_t *fdesc;
	fdesc = lookup_file_desc ( node->node );

	//we did not find the file desc in the list
	if ( !fdesc ) {
		return 0; //error
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
			if ( f_verbose >= VB_ALL ) {
				serialf ( "[FS] Error: writing to an unprivilaged file\n" );
			}
		}

	//if we have not exited yet, it is an error
	return 1;
}

u8int read_mask ( char *mask ) {
	if ( mask ) {
		u8int flags;
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

	return 0; //no mask
}

FILE *__open__ ( void *node, char *name, char *mask, u8int open ) {
	file_desc_t *tmp, *con;

	if ( f_verbose >= VB_ALL ) {
		serialf ( "[FS][OPEN]\n" );
	}

	con = initial_file_desc;

	if ( !node ) {
		return 0;
	}

	while ( con->next ) {
		//if we already have this file node in the list
		if ( con->node == node ) {
			if ( f_verbose >= VB_ALL ) {
				serialf ( "[FS][OPEN] File already opened\n" );
			}

			return con;    //no need to open, just return it
		}

		con = con->next;
	}

	tmp = ( file_desc_t* ) kmalloc ( sizeof ( file_desc_t ) );
	tmp->next = 0;


	//copy the name over
	u32int name_length = strlen ( name );
	tmp->name = ( char* ) kmalloc ( name_length + 1 ); // +1 for the \000
	memcpy ( tmp->name, name, name_length + 1 );

	tmp->node = node;
	tmp->fs_type = ( ( generic_fs_t* ) node )->magic;
	tmp->node_type = node_type ( node );
	tmp->permisions = read_mask ( mask );

	//we have the next pointer in the list.
	if ( node_fs_type ( node ) == M_VFS ) {
		if ( f_verbose >= VB_ALL ) {
			serialf ( "[FS][OPEN] VFS\n" );
		}

		tmp->inode = ( ( fs_node_t* ) node )->inode;
		tmp->size = ( ( fs_node_t* ) node )->length;

		//if it is a directory
		if ( tmp->node_type == NODE_DIRECTORY ) {
			tmp->read = 0;
			tmp->write = 0;
			tmp->finddir = ( void* ) finddir_fs;
			tmp->readdir = ( void* ) readdir_fs;

		} else {
			tmp->read = ( void* ) read_fs;
			tmp->write = ( void* ) write_fs;
			tmp->finddir = 0;
			tmp->readdir = 0;
		}
	}

	if ( open ) {
		if ( f_verbose >= VB_ALL ) {
			serialf ( "[FS][OPEN] Opened\n" );
		}

		con->next = tmp;

	}

	if ( lookup_file_desc ( ( void* ) tmp ) ) {
		if ( f_verbose >= VB_ALL ) {
			serialf ( "[FS][OPEN] Succsesful\n" );
		}
	}

	return tmp;

	if ( f_verbose >= VB_ALL ) {
		serialf ( "[FS][OPEN] failed\n" );
	}

	return 0; //cause your a fail!
}

void print_desc() {
	file_desc_t *tmp_desc;
	tmp_desc = initial_file_desc;

	for ( ; tmp_desc; tmp_desc = tmp_desc->next ) {
		if ( tmp_desc ) {
			printf ( ":%s\t%d\n", tmp_desc->name, tmp_desc->inode );
		}
	}
}

FILE *f_open ( char *filename, void *dir, char *mask ) {
	FILE *file = ( FILE* ) f_finddir ( dir, filename );

	if ( f_verbose >= VB_NAMES ) {
		serialf ( "[FS][OPEN]Opening \"%s\" inode %d ", file->name, file->inode );
	}

	if ( file ) {
		if ( f_verbose >= VB_NAMES ) {
			serialf ( "with mask: %d ", read_mask ( mask ) );
		}

		if ( read_mask ( mask ) == FDESC_READ ) {
			if ( f_verbose >= VB_ALL ) {
				serialf ( "with Read Permissions\n" );
			}

			FILE *rc = ( FILE* ) __open__ ( ( void* ) file->node, ( char* ) file->name, ( char* ) mask, 1 );
			return rc;
		}
	}

	if ( f_verbose >= VB_ALL ) {
		serialf ( "FAILED!\n" );
	}

	//if we are outside, return an error
	return 0;

}

u32int node_type ( void *node ) {
	generic_fs_t *nodet = ( generic_fs_t* ) node;
	return nodet->magic;
}

u32int node_fs_type ( void *node ) {
	generic_fs_t *nodet = ( generic_fs_t* ) node;
	return nodet->fstype;
}

FILE *f_finddir ( void *node, char *name ) {
	if ( node_type ( node ) == FS_DIRECTORY ) {
		if ( f_verbose == VB_NAMES ) {
			serialf ( "[FS][INITRD][FINDDIR] directory: %s\n", name );
		}

		switch ( ( ( generic_fs_t* ) node )->fstype ) {
		case M_UNKNOWN:
			return 0; //error

		case M_VFS: {
				//case the void * node to the vfs node structure
				fs_node_t *vfs_node = node;

				if ( f_verbose >= VB_ALL ) {
					serialf ( "[FS][INITRD][FINDDIR][VFS]\n" );
				}

				if ( vfs_node->finddir ) {
					//return an unopened file node with no r/w/a permissions at all to the actuall node data
					if ( f_verbose >= VB_ALL ) {
						serialf ( "[FS][INITRD][FINDDIR] worked\n" );
					}

					return __open__ ( vfs_node->finddir ( vfs_node, name ), name, 0, FALSE );

				} else {
					break;
				}
			}
		}
	}
}

char *name_of_dir ( void *node ) {
	return ( ( fs_node_t* ) node )->name;
}
