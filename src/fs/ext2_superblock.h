#ifndef ext2_SUPERBLOCK_H
#define ext2_SUPERBLOCK_H

//for s_state
#define	EXT2_VALID_FS	1	//Unmounted cleanly
#define	EXT2_ERROR_FS	2	//Errors detected

//for s_error
#define EXT2_ERRORS_CONTINUE	1	//continue as if nothing happened
#define EXT2_ERRORS_RO			2	//remount read-only
#define EXT2_ERRORS_PANIC		3	//cause a kernel panic

//for s_creator_os
#define EXT2_OS_LINUX	0	//Linux
#define EXT2_OS_HURD	1	//GNU HURD
#define EXT2_OS_MASIX	2	//MASIX
#define EXT2_OS_FREEBSD	3	//FreeBSD
#define EXT2_OS_LITES	4	//Lites

//for s_rev_level
#define EXT2_GOOD_OLD_REV	0	//Revision 0
#define EXT2_DYNAMIC_REV	1	//Revision 1 with variable inode sizes, extended attributes, etc.

//for s_feature_compat
#define EXT2_FEATURE_COMPAT_DIR_PREALLOC	0x0001	//Block pre-allocation for new directories
#define EXT2_FEATURE_COMPAT_IMAGIC_INODES 	0x0002
#define EXT3_FEATURE_COMPAT_HAS_JOURNAL 	0x0004	//An Ext3 journal exists
#define EXT2_FEATURE_COMPAT_EXT_ATTR	 	0x0008	//Extended inode attributes are present
#define EXT2_FEATURE_COMPAT_RESIZE_INO		0x0010	//Non-standard inode size used
#define EXT2_FEATURE_COMPAT_DIR_INDEX		0x0020	//Directory indexing (HTree)

//for s_feature_incompat
#define EXT2_FEATURE_INCOMPAT_COMPRESSION		0x0001	Disk/File compression is used
#define EXT2_FEATURE_INCOMPAT_FILETYPE			0x0002
#define EXT3_FEATURE_INCOMPAT_RECOVER			0x0004
#define EXT3_FEATURE_INCOMPAT_JOURNAL_DEV		0x0008
#define EXT2_FEATURE_INCOMPAT_META_BG			0x0010

//for s_feature_ro_compat
#define EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER	0x0001	//Sparse Superblock
#define EXT2_FEATURE_RO_COMPAT_LARGE_FILE	0x0002	//Large file support, 64-bit file size
#define EXT2_FEATURE_RO_COMPAT_BTREE_DIR	0x0004	//Binary tree sorted directory files

//for s_algo_bitmap
#define EXT2_LZV1_ALG	0	Binary value of 0x00000001
#define EXT2_LZRW3A_ALG	1	Binary value of 0x00000002
#define EXT2_GZIP_ALG	2	Binary value of 0x00000004
#define EXT2_BZIP2_ALG	3	Binary value of 0x00000008
#define EXT2_LZO_ALG	4	Binary value of 0x00000010

/*
//the ext2 superblock structure.
//interprited from http://www.nongnu.org/ext2-doc/ext2.html#SUPERBLOCK
*/
typedef struct ext2_superblock {
	extl4	s_inodes_count;
	extl4	s_blocks_count;
	extl4	s_r_blocks_count;
	extl4	s_free_blocks_count;
	extl4	s_free_inodes_count;
	extl4	s_first_data_blocks;
	extl4	s_log_block_size;
	extl4	s_log_frag_size;
	extl4	s_blocks_per_group;
	extl4	s_frags_per_group;
	extl4	s_inodes_per_group;
	extl4	s_mtime;
	extl4	s_wtime;
	extl2	s_mnt_count;
	extl2	s_max_mnt_count;
	extl2	s_magic;
	extl2	s_state;
	extl2	s_errors;
	extl2	s_minor_rev_level;
	extl4	s_lastcheck;
	extl4	s_checkinterval;
	extl4	s_creator_os;
	extl4	s_rev_level;
	extl2	s_def_resuid;
	extl2	s_def_resgid;
	//EXT2_DYNAMIC_REV
	extl4	s_first_ino;
	extl2	s_inode_size;
	extl2	s_block_group_nr;
	extl4	s_feature_compat;
	extl4	s_feature_incompat;
	extl4	s_feature_ro_compat;
	extl4	s_uuid_pt1;
	extl4	s_uuid_pt2;
	extl4	s_uuid_pt3;
	extl4	s_uuid_pt4;
	extl4	s_volume_name_pt1;
	extl4	s_volume_name_pt2;
	extl4	s_volume_name_pt3;
	extl4	s_volume_name_pt4;
	extl4	s_last_mounted_pt1;
	extl4	s_last_mounted_pt2;
	extl4	s_last_mounted_pt3;
	extl4	s_last_mounted_pt4;
	extl4	s_last_mounted_pt5;
	extl4	s_last_mounted_pt6;
	extl4	s_last_mounted_pt7;
	extl4	s_last_mounted_pt8;
	extl4	s_last_mounted_pt9;
	extl4	s_last_mounted_pt10;
	extl4	s_last_mounted_pt11;
	extl4	s_last_mounted_pt12;
	extl4	s_last_mounted_pt13;
	extl4	s_last_mounted_pt14;
	extl4	s_last_mounted_pt15;
	extl4	s_last_mounted_pt16;
	extl4	s_algo_bitmap;
	//Performance Hints
	extl1	s_prealloc_blocks;
	extl1	s_prealloc_dir_blocks;
	extl2	BUF_DO_NOT_USE1;
	//Journaling Support
	extl4	s_journal_uuid_pt1;
	extl4	s_journal_uuid_pt2;
	extl4	s_journal_uuid_pt3;
	extl4	s_journal_uuid_pt4;
	extl4	s_journal_inum;
	extl4	s_journal_dev;
	extl4	s_last_orphan;
	//Directory Indexing Support
	extl4	s_hash_seed_pt1;
	extl4	s_hash_seed_pt2;
	extl4	s_hash_seed_pt3;
	extl4	s_hash_seed_pt4;
	extl1	s_def_hash_version;
	extl1	BUF_DO_NOT_USE2;
	extl2	BUF_DO_NOT_USE3;
	//Other Options
	extl4	s_default_mount_options;
	extl4	s_first_meta_bg;
	//760 bytes left
} ext2_superblock_t;

#endif //ext2_SUPERBLOCK_H
