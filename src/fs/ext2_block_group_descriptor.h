#ifndef ext2_BLOCK_GROUP_DESCRIPTOR_H
#define ext2_BLOCK_GROUP_DESCRIPTOR_H

typedef struct ext2_blockgroupdescriptor {
	extl4	bg_block_bitmap;
	extl4	bg_inode_bitmap;
	extl4	bg_inode_table;
	extl2	bg_free_blocks_count;
	extl2	bg_free_inodes_count;
	extl2	bg_used_dirs_count;
	extl2	bg_pad;
	//Reserved 12
} ext2_blockgroupdescriptor_t;

#endif//ext2_BLOCK_GROUP_DESCRIPTOR_H
