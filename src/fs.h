#ifndef FS_H
#define FS_H

#include "common.h"
#include "vfs.h"

///node type values
enum 
{
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

/*for the file desctptor*/
typedef struct file_desc
{
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
  u32int (*_read)(void*, u32int, u32int, u8int*);
  u32int (*_write)(void*, u32int, u32int, u8int*);
  u32int (*_finddir)(void*, char*);
  u32int (*_readdir)(void*, u32int);

  //pointing to the next structure of the list
  struct file_desc *next;
} file_desc_t;

typedef struct file_desc FILE;

file_desc_t *lookup_file_desc(void *node);

u32int f_read(FILE file, u32int offset, u32int size, u8int *buffer);

#endif
