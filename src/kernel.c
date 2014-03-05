// kernel.c -- MatrixOS kernel

#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "paging.h"
#include "multiboot.h"
#include "fs.h"
#include "initrd.h"
#include "task.h"
#include "syscall.h"
#include "colour.h"

#include "input/keyboard.h"
#include "system/moduleLoading.h"
#include "graphics/graphics.h"
#include "system/shell.h"

#define VER_MAJOR		1
#define VER_MINOR		1
#define VER_FIX			1

//#define TASKING_USER

extern u32int placement_address;
u32int initial_esp;
struct multiboot *mboot_ptr;

void init();
void print_version();
s8int locate_initrd();
s8int init_Interupts();

/*
// Call asm 'sti'.
// This is a syscall function.
*/
void sti() {
	asm volatile ( "sti" );
}

/*
// This is where everything starts...
// If you dont understand this go learn osDeving
*/
int kernel_main ( struct multiboot *mboot_point, u32int initial_stack ) {
	monitor_clear();
	initial_esp = initial_stack;
	mboot_ptr = mboot_point;

	VGA_init ( 320, 200, 256 );

	monitor_set_cursor_pos ( 0, 0 );
	monitor_set_fore_colour ( GREEN );
	monitor_write ( "MatrixOS" );
	monitor_set_cursor_pos ( 10, 0 );
	print_version();
	monitor_write ( "\n" );
	monitor_set_fore_colour ( WHITE );

	init();

	//startShell();

	for ( ;; ) {}
}

/*
// Print the current version of MatrixOS in COLOUR!!!
// Is printed wherever the cursor is.
*/
void print_version() {
	if ( VER_MINOR == 0 && VER_FIX == 0 ) {
		monitor_set_back_colour ( 1 );
	}

	if ( VER_MINOR >= 1 && VER_FIX == 0 ) {
		monitor_set_back_colour ( 12 );
	}

	if ( VER_FIX >= 1 ) {
		monitor_set_back_colour ( 4 );
	}

	monitor_write ( "v" );
	monitor_set_fore_colour ( 10 ); //green
	monitor_write_dec ( VER_MAJOR );
	monitor_write ( "." );
	monitor_set_fore_colour ( 14 ); //yellow
	monitor_write_dec ( VER_MINOR );
	monitor_set_fore_colour ( 10 ); //green
	monitor_write ( "." );
	monitor_set_fore_colour ( 12 ); //red
	monitor_write_dec ( VER_FIX );
	monitor_set_fore_colour ( 15 );
	monitor_set_back_colour ( 0 );
}

/*
// Stick your lowlevel initalisation in here!
*/
void init() {
	SYSCALL_ENABLED =	0;
	SLEEP_ENABLED =		0;

	// Initialise all the ISRs and segmentation
	runModule ( &init_descriptor_tables );

	asm volatile ( "sti" );
	runModule ( &initialise_syscalls );

	runModule ( &init_Interupts );

	// Initialise the PIT to 50Hz
	asm volatile ( "sti" );
	init_timer ( 50 );

	runModule ( &locate_initrd );

	// Start paging.
	runModule ( &initialise_paging );

	runModule ( &init_keyboard );

#ifdef TASKING_USER
	// Start multitasking.
	runModule ( &initialise_tasking );
	runModule ( &switch_to_user_mode );
#endif

	return;
}

/*
// Find the Initrd and remeber where it is!
*/
s8int locate_initrd() {
	syscall_monitor_write ( "Locating Initrd." );
	// Find the location of our initial ramdisk.
	ASSERT ( mboot_ptr->mods_count > 0 );
	u32int initrd_location = * ( ( u32int* ) mboot_ptr->mods_addr );
	u32int initrd_end = * ( u32int* ) ( mboot_ptr->mods_addr+4 );
	// Don't trample our module with placement accesses, please!
	placement_address = initrd_end;
	// Initialise the initial ramdisk, and set it as the filesystem root.
	fs_root = initialise_initrd ( initrd_location );
	return 0;
}

void General_Protection_Fault ( registers_t *regs ) {
	PANIC ( "General Protection Fault" );
}

s8int init_Interupts() {
	syscall_monitor_write ( "Initalizing Interupts." );
	register_interrupt_handler ( 13, General_Protection_Fault );
	return 0;
}
