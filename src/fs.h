// MatrixOS - fs.h
// Filesystem main services heder
#ifndef FS_H
#define FS_H

#include "common.h"
#include "vfs.h"

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
	NODE_UNKOWN,
	NODE_FILE,
	NODE_DIRECTORY,
	NODE_CHARD_DEV,
	NODE_BLOCK_DEV,
	NODE_FIFO,
	NODE_SOCKET,
	NODE_SYMLINK,
	NODE_HARDLINK,
	NODE_MOUNTPOINT
};

typedef u32int ( *read_fs_t ) ( void*, u32int, u32int, u8int* );
typedef u32int ( *write_fs_t ) ( void*, u32int, u32int, u8int* );
typedef u32int ( *finddir_fs_t ) ( void*, char* );
typedef u32int ( *readdir_fs_t ) ( void*, u32int );

/*for the file desctptor*/
typedef struct file_desc {
	char *name;
	u32int fs_type;     //is it VFS, EXT2, etc.
	u32int node_type;   //is it a file, directory, ect.
	u8int permisions;
	u32int inode;
	void *node;
	u32int size;

	//callback operations
	read_fs_t *read;
	write_fs_t *write;
	finddir_fs_t *finddir;
	readdir_fs_t *readdir;

	//pointing to the next structure of the list
	struct file_desc *next;
} file_desc_t;

typedef struct file_desc FILE;

typedef struct generic_fs {
	u8int magic;
	u32int fstype;
} generic_fs_t;

s8int init_file_system();

s8int init_initrd_desc();

file_desc_t *lookup_file_desc ( void *node );

u32int f_read ( file_desc_t *file, u32int offset, u32int size, u8int *buffer );

u32int f_write ( FILE *node, u32int offset, u32int size, u8int *buffer );

FILE *__open__ ( void *node, char *name, char *mask, u8int open );

FILE *f_open ( char *filename, void *dir, char *mask );

FILE *f_finddir ( void *node, char *name );

u32int node_type ( void *node );

u32int node_fs_type ( void *node );

char *name_of_dir ( void *node );

void print_desc();

#endif
