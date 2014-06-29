// kernel.c -- MatrixOS kernel

#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "paging.h"
#include "multiboot.h"
#include "fs.h"
#include "vfs.h"
#include "initrd.h"
#include "task.h"
#include "syscall.h"
#include "colour.h"

#include "input/keyboard.h"
#include "system/moduleLoading.h"
#include "graphics/graphics.h"
#include "system/shell.h"
#include "elf_loader.h"
#include "fs/ext2.h"

#define VER_MAJOR		1
#define VER_MINOR		3
#define VER_FIX			0

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

	init_serial ( 1, 9 );

	//VGA_init ( 320, 200, 256 );
	//VGA_init(1024, 768, 24);

	monitor_set_cursor_pos ( 0, 0 );
	monitor_set_fore_colour ( GREEN );
	kprintf ( "MatrixOS " );
	monitor_set_cursor_pos ( 11, 0 );
	print_version();
	kprintf ( "\n" );
	monitor_set_fore_colour ( WHITE );


	//printf ( "Address: %h\n", mboot_ptr->addr );

	init();

	test();

	/*FILE *q;
	q = ( FILE* ) f_open ( "1", fs_root, "r" );
	serialf ( "RETURNED\n" );
	char *buf = ( char* ) kmalloc ( sizeof ( char ) * 150 );
	f_read ( q, 0, 150, buf );
	serialf("\n\n%s\n", buf);*/

	startShell();

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

	kprintf ( "v" );
	monitor_set_fore_colour ( 10 ); //green
	kprintf ( "%d", VER_MAJOR );
	kprintf ( "." );
	monitor_set_fore_colour ( 14 ); //yellow
	kprintf ( "%d", VER_MINOR );
	monitor_set_fore_colour ( 10 ); //green
	kprintf ( "." );
	monitor_set_fore_colour ( 12 ); //red
	kprintf ( "%d", VER_FIX );
	monitor_set_fore_colour ( 15 );
	monitor_set_back_colour ( 0 );
}

void mehpid() {
	printf ( "%s pid is: %d\n", gettaskname(), getpid() );
	exit();
}

void clock() {
	while ( 1 ) {
		u8int oldX = monitor_getX();
		u8int oldY = monitor_getY();
		monitor_set_cursor_pos ( 70, 0 );
		datetime_t system_time = getLocaltime ( "sydney" );
		printf ( "%d:%d:%d  ", system_time.hour, system_time.min, system_time.sec );
		monitor_set_cursor_pos ( oldX, oldY );
		sleep ( 1 );
	}

	exit();
}

/*
// Stick your lowlevel initalisation in here!
*/
extern fs_node_t* fs_root;

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

	runModule ( &init_file_system );

	runModule ( &locate_initrd );

	// Start paging.
	runModule ( &initialise_paging );

	runModule ( &init_keyboard );

	datetime_t system_time = getDatetime();

	printf ( "[Time] is %d:%d:%d %d/%d/%d\n", system_time.sec, system_time.min, system_time.hour, system_time.day, system_time.month, system_time.year );

	// Start multitasking.
	runModule ( &initialise_tasking );
	enable_tasking();
	monitor_set_fore_colour ( 12 );
	printf ( "%s pid is: %d\n", gettaskname(), getpid() );
	start_task ( 200, 10, clock, 0, "UserClock" );
	//sleep ( 1 );
	monitor_set_fore_colour ( 15 );


	//start_elf ( "elf_test" );
	//load_elf ( "testbin" );
	//load_elf("hello");

	//printf ( "FINISHED!\n" );

	//runModule ( &switch_to_user_mode);

}

/*
// Find the Initrd and remeber where it is!
*/
s8int locate_initrd() {
	printf ( "Locating Initrd." );
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
	disable_tasking();
	PANIC ( "General Protection Fault" );
}

s8int init_Interupts() {
	printf ( "Initalizing Interupts." );
	register_interrupt_handler ( 13, General_Protection_Fault );
	return 0;
}
