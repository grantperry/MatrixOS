#ifndef VFS_H
#define VFS_H

#include "common.h"
#include "initrd.h"

// used for the flags in "fs_node_t"
#define FS_FILE        0x01
#define FS_DIRECTORY   0x02
#define FS_CHARDEVICE  0x03
#define FS_BLOCKDEVICE 0x04
#define FS_PIPE        0x05
#define FS_SYMLINK     0x06
#define FS_MOUNTPOINT  0x08 // Is the file an active mountpoint?


struct fs_node;

// These typedefs define the type of callbacks - called when read/write/open/close
// are called.
typedef u32int ( *read_type_t ) ( struct fs_node*,u32int,u32int,u8int* );
typedef u32int ( *write_type_t ) ( struct fs_node*,u32int,u32int,u8int* );
typedef void ( *open_type_t ) ( struct fs_node* );
typedef void ( *close_type_t ) ( struct fs_node* );
typedef struct dirent * ( *readdir_type_t ) ( struct fs_node*,u32int );
typedef struct fs_node * ( *finddir_type_t ) ( struct fs_node*,char *name );

typedef struct permissions {
	u8int read;
	u8int write;
	u8int exec;
} permissions_t;

typedef struct fs_block {
	u32int length;
	char pointer;
	struct fs_block *next;
} fs_block_t;

struct fs_node {
	char name[128];     // The filename.
	u32int mask;        // The permissions mask.
	u32int uid;         // The owning user.
	u32int gid;         // The owning group.
	u32int flags;       // Includes the node type. See #defines above.
	u32int inode;       // file identifier. used in this device only
	u32int impl;        // An implementation-defined number.
	u32int node_type;	// is it a file, directory, symlink

	u32int length;      // Size of the file, in bytes. add up lengths in linked list to check...
	struct fs_block *pointer; //for files
	struct dirent *dpointer; //for directories

	permissions_t permissions;
	read_type_t read;
	write_type_t write;
	open_type_t open;
	close_type_t close;
	readdir_type_t readdir;
	finddir_type_t finddir;
	u32int num_nodes;		/* Number of child nodes */
	u32int seek;
	struct fs_node *ptr; // Used by mountpoints and symlinks.
};

typedef struct fs_node fs_node_t;

struct dirent {
	u32int file_type;
	u32int rec_len;
	u32int name_len;
	u32int ino;     // Inode number. Required by POSIX.
	char *name; // Filename.
};

// Standard read/write/open/close functions. Note that these are all suffixed with
// _fs to distinguish them from the read/write/open/close which deal with file descriptors
// , not file nodes.
u32int read_fs ( fs_node_t *node, u32int offset, u32int size, u8int *buffer );
u32int write_fs ( fs_node_t *node, u32int offset, u32int size, u8int *buffer );
void open_fs ( fs_node_t *node, u8int read, u8int write );
void close_fs ( fs_node_t *node );
struct dirent *readdir_fs ( fs_node_t *node, u32int index );
fs_node_t *finddir_fs ( fs_node_t *node, char *name );
fs_node_t *vfs_createFile ( fs_node_t *parentNode, char *name, u32int size );
int findOpenNode();
u32int *block_of_set ( fs_node_t *node, u32int block_number );

extern fs_node_t *fs_root; // The root of the filesystem.

#endif//VFS_H
