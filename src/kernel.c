#include "monitor.h"
#include "common.h"
#include "timer.h"

void sleep(u32int);

int kernel_main(struct multiboot *mboot_ptr) {
	monitor_clear();
	init_descriptor_tables();
	init_interupts();
	init_timer();
	monitor_clear();
	initialise_paging();
	sleep(100);
   
	monitor_write("#------------------------------------------------------------------------------#");
	monitor_write("| Welcome To MatrixOS                                                          |");
	monitor_write("#------------------------------------------------------------------------------#");
	monitor_write("> ");

	return 0;
}
