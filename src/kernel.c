#include "monitor.h"
#include "common.h"
#include "timer.h"
#include "multiboot.h"
#include "fs.h"
#include "initrd.h"

extern u32int placement_address;

int kernel_main(struct multiboot *mboot_ptr) {
	monitor_clear();
	init_descriptor_tables();
	init_interupts();
	init_timer();
	monitor_clear();
	initialise_paging();
	sleep(10);
	
	
	// Find the location of our initial ramdisk.
   ASSERT(mboot_ptr->mods_count > 0);
   u32int initrd_location = *((u32int*)mboot_ptr->mods_addr);
   u32int initrd_end = *(u32int*)(mboot_ptr->mods_addr+4);
   // Don't trample our module with placement accesses, please!
   placement_address = initrd_end;
   
	monitor_write("#------------------------------------------------------------------------------#");
	monitor_write("| Welcome To MatrixOS!                                                         |");
	monitor_write("#------------------------------------------------------------------------------#");
	monitor_write("> ");
	
	// Initialise the initial ramdisk, and set it as the filesystem root.
   fs_root = initialise_initrd(initrd_location);
   
   // list the contents of /
int i = 0;
struct dirent *node = 0;
while ( (node = readdir_fs(fs_root, i)) != 0)
{
  monitor_write("Found file ");
  monitor_write(node->name);
  fs_node_t *fsnode = finddir_fs(fs_root, node->name);

  if ((fsnode->flags&0x7) == FS_DIRECTORY)
    monitor_write("\n\t(directory)\n");
  else
  {
    monitor_write("\n\t contents: \"");
    char buf[256];
    u32int sz = read_fs(fsnode, 0, 256, buf);
    int j;
    for (j = 0; j < sz; j++)
      monitor_put(buf[j]);

    monitor_write("\"\n");
  }
  i++;
}

	return 0;
}
