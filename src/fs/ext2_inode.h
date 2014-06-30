#ifndef ext2_INODE_H
#define ext2_INODE_H

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

typedef struct ext2_inode {
	extl2	i_mode;
	extl2	i_uid;
	extl4	i_size;
	extl4	i_atime;
	extl4	i_ctime;
	extl4	i_mtime;
	extl4	i_dtime;
	extl2	i_gid;
	extl2	i_link_count;
	extl4	i_blocks;
	extl4	i_flags;
	extl4	i_osd1;
	inode_block_t	i_block;
	extl4	i_generation;
	extl4	i_file_acl;
	extl4	i_dir_acl;
	extl4	i_faddr;
	extl4	i_osd2_1;
	extl4	i_osd2_2;
	extl4	i_osd2_3;
} ext2_inode_t;

#endif //ext2_INODE_H
