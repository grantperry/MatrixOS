#include "monitor.h"
#include "common.h"

int kernel_main(struct multiboot *mboot_ptr) {
	monitor_clear();
	init_descriptor_tables();
	init_interupts();
	monitor_clear();
	monitor_write("#------------------------------------------------------------------------------#");
	monitor_write("| Welcome To MatrixOS                                                          |");
	monitor_write("#------------------------------------------------------------------------------#");
	monitor_write("> ");
	asm volatile("cli");
	init_timer(1193180); // Initialise timer to 50Hz
	asm volatile("sti");
	return 0;
}
