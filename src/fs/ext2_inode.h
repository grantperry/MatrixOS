#ifndef ext2_INODE_H
#define ext2_INODE_H

#define EXT2_BAD_INO			1	//bad blocks inode
#define EXT2_ROOT_INO			2	//root directory inode
#define EXT2_ACL_IDX_INO		3	//ACL index inode (deprecated?)
#define EXT2_ACL_DATA_INO		4	//ACL data inode (deprecated?)
#define EXT2_BOOT_LOADER_INO	5	//boot loader inode
#define EXT2_UNDEL_DIR_INO		6	//undelete directory inode

//for i_mode
//-- file format --
#define EXT2_S_IFSOCK	0xC000	//socket
#define EXT2_S_IFLNK	0xA000	//symbolic link
#define EXT2_S_IFREG	0x8000	//regular file
#define EXT2_S_IFBLK	0x6000	//block device
#define EXT2_S_IFDIR	0x4000	//directory
#define EXT2_S_IFCHR	0x2000	//character device
#define EXT2_S_IFIFO	0x1000	//fifo
//-- process execution user/group override --
#define EXT2_S_ISUID	0x0800	//Set process User ID
#define EXT2_S_ISGID	0x0400	//Set process Group ID
#define EXT2_S_ISVTX	0x0200	//sticky bit
//-- access rights --
#define EXT2_S_IRUSR	0x0100	//user read
#define EXT2_S_IWUSR	0x0080	//user write
#define EXT2_S_IXUSR	0x0040	//user execute
#define EXT2_S_IRGRP	0x0020	//group read
#define EXT2_S_IWGRP	0x0010	//group write
#define EXT2_S_IXGRP	0x0008	//group execute
#define EXT2_S_IROTH	0x0004	//others read
#define EXT2_S_IWOTH	0x0002	//others write
#define EXT2_S_IXOTH	0x0001	//others execute

//for i_flags
#define EXT2_SECRM_FL			0x00000001	//secure deletion
#define EXT2_UNRM_FL			0x00000002	//vrecord for undelete
#define EXT2_COMPR_FL			0x00000004	//compressed file
#define EXT2_SYNC_FL			0x00000008	//synchronous updates
#define EXT2_IMMUTABLE_FL		0x00000010	//immutable file
#define EXT2_APPEND_FL			0x00000020	//append only
#define EXT2_NODUMP_FL			0x00000040	//do not dump/delete file
#define EXT2_NOATIME_FL			0x00000080	//do not update .i_atime
//-- Reserved for compression usage --
#define EXT2_DIRTY_FL			0x00000100	//Dirty (modified)
#define EXT2_COMPRBLK_FL		0x00000200	//compressed blocks
#define EXT2_NOCOMPR_FL			0x00000400	//access raw compressed data
#define EXT2_ECOMPR_FL			0x00000800	//compression error
//-- End of compression flags --
#define EXT2_BTREE_FL			0x00001000	//b-tree format directory
#define EXT2_INDEX_FL			0x00001000	//hash indexed directory
#define EXT2_IMAGIC_FL			0x00002000	//AFS directory
#define EXT3_JOURNAL_DATA_FL	0x00004000	//journal file data
#define EXT2_RESERVED_FL		0x80000000	//reserved for ext2 library

typedef struct inode_block_int { //15 x 32 bits
	extl4	block_1;
	extl4	block_2;
	extl4	block_3;
	extl4	block_4;
	extl4	block_5;
	extl4	block_6;
	extl4	block_7;
	extl4	block_8;
	extl4	block_9;
	extl4	block_10;
	extl4	block_11;
	extl4	block_12;
	extl4	block_13;
	extl4	block_14;
	extl4	block_15;
} inode_block_t;

typedef struct osd2_linux {
	extl1	l_i_frag;
	extl1	l_i_gsize;
	extl2	PACK;
	extl2	l_i_uid_high;
	extl2	l_i_gid_high;
	extl4	RESERVED;
} osd2_linux_t;

typedef struct ext2_inode {
	extl2			i_mode;
	extl2			i_uid;
	extl4			i_size;
	extl4			i_atime;
	extl4			i_ctime;
	extl4			i_mtime;
	extl4			i_dtime;
	extl2			i_gid;
	extl2			i_link_count;
	extl4			i_blocks;
	extl4			i_flags;
	extl4			i_osd1;
	inode_block_t	i_block;
	extl4			i_generation;
	extl4			i_file_acl;
	extl4			i_dir_acl;
	extl4			i_faddr;
	osd2_linux_t	i_osd2;
} ext2_inode_t;

#endif //ext2_INODE_H
