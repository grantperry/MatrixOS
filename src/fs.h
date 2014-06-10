#ifndef FS_H
#define FS_H

#include "common.h"

//for the file descriptor
#define FDESC_CLEAR   0b1000
#define FDESC_READ    0b100
#define FDESC_WRITE   0b10
#define FDESC_APPEND  0b1

//formats
#define M_UNKNOWN       0
#define M_VFS           1
#define M_EXT2          2

///node type values
enum {
	nTYPE_UNKOWN,
	nTYPE_FILE,
	nTYPE_DIRECTORY,
	nTYPE_CHARD_DEV,
	nTYPE_BLOCK_DEV,
	nTYPE_FIFO,
	nTYPE_SOCKET,
	nTYPE_SYMLINK,
	nTYPE_HARDLINK,
	nTYPE_MOUNTPOINT
};

typedef u32int ( *read_fs_t ) ( void*, u32int, u32int, u8int* );
typedef u32int ( *write_fs_t ) ( void*, u32int, u32int, u8int* );
typedef u32int ( *finddir_fs_t ) ( void*, char* );
typedef u32int ( *readdir_fs_t ) ( void*, u32int );

/*for the file desctptor*/
typedef struct file_desc {
	char *name;
	u32int name_length;
	void *node;
	u32int fs_type;     //is it VFS, EXT2, etc.
	u32int node_type;   //is it a file, directory, ect.
	u8int permisions;
	s8int inode;
	u32int size;

	//callback operations
	//~ u32int (*_close)(void*);
	read_fs_t *read;
	write_fs_t *write;
	finddir_fs_t *finddir;
	readdir_fs_t *readdir;

	//pointing to the next structure of the list
	struct file_desc *next;
} file_desc_t;

typedef struct file_desc FILE;

s8int init_file_system();

file_desc_t *lookup_file_desc ( void *node );

u32int f_read ( file_desc_t *file, u32int offset, u32int size, u8int *buffer );

u32int f_write ( FILE *node, u32int offset, u32int size, u8int *buffer );

static u8int __open_fs_mask_to_u32int__ ( char *mask );

FILE *__open__ ( void *node, char *name, char *mask, u8int open );

FILE *f_open ( char *filename, void *dir, char *mask );

FILE *f_finddir ( void *node, char *name );

u32int node_type ( void *node );

char *name_of_dir ( void *node );

void print_desc();

#endif
