// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
//		   Made for JamesM's tutorials <www.jamesmolloy.co.uk>

#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "paging.h"
#include "multiboot.h"
#include "fs.h"
#include "initrd.h"
#include "task.h"
#include "syscall.h"

#include "input/keyboard.h"

extern u32int placement_address;
u32int initial_esp;
struct multiboot *mboot_ptr;

void init();

int kernel_main(struct multiboot *mboot_point, u32int initial_stack)
{
	monitor_clear();
	initial_esp = initial_stack;
	mboot_ptr = mboot_point;
	monitor_write("#------------------------------------------------------------------------------#");
	monitor_write("|  Welcome to MatrixOS!!!                                                 v1.0 |");
	monitor_write("#------------------------------------------------------------------------------#");
	monitor_write("> ");
	init();
	
	print("Hello, user world!\n");
	
	return 0;
}

void init() {

	// Initialise all the ISRs and segmentation
	init_descriptor_tables();
	// Initialise the PIT to 100Hz
	asm volatile("sti");
	init_timer(50);

	// Find the location of our initial ramdisk.
	ASSERT(mboot_ptr->mods_count > 0);
	u32int initrd_location = *((u32int*)mboot_ptr->mods_addr);
	u32int initrd_end = *(u32int*)(mboot_ptr->mods_addr+4);
	// Don't trample our module with placement accesses, please!
	placement_address = initrd_end;

	// Start paging.
	initialise_paging();

	// Start multitasking.
	initialise_tasking();

	// Initialise the initial ramdisk, and set it as the filesystem root.
	fs_root = initialise_initrd(initrd_location);

	initialise_syscalls();


	init_keyboard();

	switch_to_user_mode();
}
