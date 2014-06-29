#ifndef ext2_H
#define ext2_H

typedef	unsigned	char	extl1;
typedef	unsigned	short	extl2;
typedef	unsigned	int		extl4;

typedef struct ext2_superblock {
	extl4	s_inodes_count;
	extl4	s_blocks_count;
	extl4	s_r_blocks_count;
	extl4	s_free_blocks_count;
	extl4	s_free_inodes_count;
} ext2_superblock_t;

#endif //ext2_H
