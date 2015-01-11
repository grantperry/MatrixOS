// MatrixOS - kernel.c
// Woooo.
// The heart of MatrixOS sits here.

#include <monitor.h>
#include <descriptor_tables.h>
#include <timer.h>
#include <paging.h>
#include <multiboot.h>
#include <fs.h>
#include <vfs.h>
#include <initrd.h>
#include <task.h>
#include <syscall.h>
#include <colour.h>

#include <keyboard.h>
#include <moduleLoading.h>
#include <shell.h>
#include <ext2.h>
#include <sound.h>
#include <elf.h>
#include <VESA/VESA.h>

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
// Returns The lower and upper memory up to the first memory hole.
// Returns in KB.
*/
u32int return_memory() {
	u32int mem = mboot_ptr->mem_lower + mboot_ptr->mem_upper;
	return mem;
}

void tsk() {
	//serialf("Hello from usermode task\n");
	//syscall_sleep(1);
	exit();
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

	monitor_set_cursor_pos ( 0, 0 );
	monitor_set_fore_colour ( GREEN );
	kprintf ( "MatrixOS " );
	monitor_set_cursor_pos ( 11, 0 );
	print_version();
	kprintf ( "\n" );
	monitor_set_fore_colour ( WHITE );

	init();

	init_PCI();

	set_f_verbose ( VB_NONE );

	//elf("binary.elf");

	setDisplay(0x118);
	
	drawRect(20, 40, 200, 400, 0x0088FF33);
	sleep(1);
	drawRect(25, 45, 190, 390, 0x000000FF);
	sleep(1);
	u32int r = openBMP("lena.bmp");
	serialf("bmp:ret: %d\n", r);

	startShell();

	switch_to_user_mode();

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

void init_sound() {
	play_sound ( 1000 );
	sleep ( 1 );
	play_sound ( 990 );
	sleep ( 1 );
	play_sound ( 1000 );
	sleep ( 1 );
	nosound();
	exit();
}

s8int init_sound_multitsk() {
	printf ( "Initiating sound for multitasking environment." );
	start_task ( 200, 10, init_sound, 0, "init_sound_test" );
	return 0;
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
	asm volatile ( "sti" );

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

	runModule ( &init_sound_multitsk );

	printf ( "running flat binary at 0x500000\n" );
	load_flat ( "flat.bin", 0x500000 );
	load_flat ( "flat2.bin", 0x501000 );

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
