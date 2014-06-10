#ifndef VFS_H
#define VFS_H

#include "common.h"

struct fs_node;

#define FS_FILE        0x01
#define FS_DIRECTORY   0x02
#define FS_CHARDEVICE  0x03
#define FS_BLOCKDEVICE 0x04
#define FS_PIPE        0x05
#define FS_SYMLINK     0x06
#define FS_MOUNTPOINT  0x08 // Is the file an active mountpoint?

#define FILE_LEN		8
#define DIR_LEN			FILE_LEN


// These typedefs define the type of callbacks - called when read/write/open/close
// are called.
typedef u32int ( *read_type_t ) ( struct fs_node*,u32int,u32int,u8int* );
typedef u32int ( *write_type_t ) ( struct fs_node*,u32int,u32int,u8int* );
typedef void ( *open_type_t ) ( struct fs_node* );
typedef void ( *close_type_t ) ( struct fs_node* );
typedef struct dirent * ( *readdir_type_t ) ( struct fs_node*,u32int );
typedef struct fs_node * ( *finddir_type_t ) ( struct fs_node*,char *name );

typedef struct permision {
	u8int read;
	u8int write;
	u8int execute;
} permission_t;

typedef struct fs_node {
	u32int name_len;	//lenght of the file name
	char *name;     // The filename.
	u32int mask;        // The permissions mask.
	u32int uid;         // The owning user.
	u32int gid;         // The owning group.
	u32int flags;       // Includes the node type. See #defines above.
	u32int inode;       // This is device-specific - provides a way for a filesystem to identify files.

	permission_t permissions; //TODO Remove this

	u32int *pointer;

	u32int length;      // Size of the file, in bytes.
	u32int impl;        // An implementation-defined number.

	read_type_t read;
	write_type_t write;
	open_type_t open;
	close_type_t close;
	readdir_type_t readdir;
	finddir_type_t finddir;
	u32int num_nodes;		/* Number of child nodes */
	u32int seek;

	struct fs_node *ptr; // Used by mountpoints and symlinks.
} fs_node_t;

struct dirent {
	u32int name_len;//length of name
	u32int rec_len;
	u8int type;
	char *name; // Filename. malloc this to give ptr.
	u32int ino;     // Inode number. Required by POSIX.
};

extern fs_node_t *fs_root; // The root of the filesystem.

u32int read_fs ( fs_node_t *node, u32int offset, u32int size, u8int *buffer );

u32int write_fs ( fs_node_t *node, u32int offset, u32int size, u8int *buffer );

struct dirent *readdir_fs ( fs_node_t *node, u32int index );

fs_node_t *finddir_fs ( fs_node_t *node, char *name );

int findOpenNode();

void open_fs ( fs_node_t *node, char rw );

void close_fs ( fs_node_t *node );

#endif //VFS_H
