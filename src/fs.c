#include "fs.h"
#include "vfs.h"

//the curent directory node
void *currentDirectory;

//initial file descriptor location
file_desc_t *initial_file_desc;

s8int init_file_system() {
	printf ( "Initalizing File System." );
	initial_file_desc = ( file_desc_t* ) kmalloc ( sizeof ( file_desc_t ) );
	strcpy ( initial_file_desc->name, "root" );
	initial_file_desc->next = 0;
	
	return 0;
}

/*
// Checks weather the file is already open or not.
*/
file_desc_t *lookup_file_desc ( void *node ) {
	file_desc_t *temp_desc;
	temp_desc = initial_file_desc; //make a copy so we dont muck it up.
	serialf("[FS] file name: %s and node: %d\n", temp_desc->next->next->name, temp_desc->next->next->inode);

	for ( ; temp_desc->node != node && temp_desc; temp_desc = temp_desc->next ) {
	}
	int i = 1;
	while ( i ) {
		if (temp_desc->node == node && !temp_desc) {
			break;
		}
		temp_desc = temp_desc->next;
	}
	//serialf("[FS] file name: %s and node: %d\n", temp_desc->name, temp_desc->inode);

	if ( !temp_desc ) {
		serialf ( "[FS] File lookup failed!\n" );
		return 0; //descriptor is null, return 0.
	}

	return temp_desc;
}

//TODO actually read the file
u32int f_read ( file_desc_t *file, u32int offset, u32int size, u8int *buffer ) {
	file_desc_t *fdesc;
	fdesc = ( file_desc_t* ) lookup_file_desc ( file->node );

	//we did not find the file desc in the list
	if ( !fdesc ) {
		serialf ( "[FS] File Not Opened!\n" );
		return 0; //error
	}

	serialf ( "[FS] Reading File %s\n", fdesc->name );

	if ( ! ( fdesc->permisions & FDESC_READ ) ) {
		serialf ( "[FS] No Read Permisions\n" );
		return 0;
	}

	if ( fdesc->fs_type == M_UNKNOWN ) {
		serialf ( "[FS] node has no known callbacks!\n" );
		return 0;
	}

	if ( fdesc->fs_type == M_VFS ) {
		serialf ( "[FS] Reading from VFS\n" );

		if ( ! ( ( ( fs_node_t* ) fdesc->node )->read ) ) {
			serialf ( "[FS] FAILED. No callback\n" );
		}

		serialf ( "[FS] Callback found!\n" );
	}

	if ( fdesc->fs_type == M_EXT2 ) {
		//return ext2_read(fdesc->node, offset, size, buffer);
		serialf ( "[FS] TODO Support ext2-4\n" );
	}

	serialf ( "[FS] File is not linked to a Filesystem" );
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
			serialf ( "[FS] Error: writing to an unprivilaged file\n" );
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
	if ( node ) {
		file_desc_t *tmp_desc;
		tmp_desc = initial_file_desc;

		if ( !tmp_desc ) {
			serialf ( "[FS] no file descriptor " );
			return 0;
		}

		for ( ; tmp_desc->next; tmp_desc = tmp_desc->next ) {
			if ( &tmp_desc == 0 ) {
				break;
			}

			if ( tmp_desc->node == node ) {
				return tmp_desc; //no point in returning error
			}
		}

		file_desc_t *new_desc; //new descriptor for the list
		new_desc = ( file_desc_t* ) kmalloc ( sizeof ( file_desc_t ) );
		u16int new_name_len;
		new_name_len = strlen ( name ); //get the name and transfer it.
		new_desc->name = ( char* ) kmalloc ( new_name_len + 1 ); // +1 for the \000
		memcpy ( new_desc->name, name, new_name_len + 1 ); // save the name
		new_desc->name_length = new_name_len; //save the length...
		new_desc->node = node; //save the directory node
		//TODO set FS TYPE!
		new_desc->fs_type = M_VFS; //TODO set real FS Type
		//TODO set NODE TYPE!
		new_desc->permisions = read_mask ( mask );
		new_desc->next = 0; //so we dont walk of the end.

		if ( read_mask ( mask ) & ( FDESC_READ ) ) {
			serialf ( "[FS][OPEN] %s Read callback set\n", new_desc->name );
			new_desc->read = ( read_fs_t* ) read_fs;
		}

		if ( read_mask ( mask ) & ( FDESC_WRITE ) ) {
			serialf ( "[FS][OPEN] %s Write callback set\n", new_desc->name );
			new_desc->write = ( write_fs_t* ) write_fs;
		}

		if ( open == TRUE ) {
			tmp_desc->next = ( file_desc_t* ) new_desc;
		}

		return new_desc;
	}

	return 0; //cause your a fail!
}

void print_desc() {
	file_desc_t *tmp_desc;
	tmp_desc = initial_file_desc;

	for ( ; tmp_desc; tmp_desc = tmp_desc->next ) {
		if ( tmp_desc ) {
			printf ( ":%s\n", tmp_desc->name );
		}
	}
}

FILE *f_open ( char *filename, void *dir, char *mask ) {
	if ( !dir ) {
		return 0;    //error
	}

	struct dirent *d;
	d = (struct dirent*) readdir_fs(fs_root, 1);
	serialf("name1: %s\n", d->name);

	FILE *file;
	file = ( FILE * ) f_finddir ( (fs_node_t*) dir, filename );
	
	
	//a file already exists to be opened
	if ( file ) {
		return __open__ ( file->node, filename, mask, TRUE );
	}

	//if we are outside, return an error
	return 0;

}

FILE *f_finddir ( void *node, char *name ) {
	return (FILE*)finddir_fs(node, name);
}

char *name_of_dir ( void *node ) {
	return ( ( fs_node_t* ) node )->name;
}
