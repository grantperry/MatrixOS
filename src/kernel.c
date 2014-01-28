#include "monitor.h"
#include "common.h"
#include "timer.h"

void sleep(u32int);

int kernel_main(struct multiboot *mboot_ptr) {
	monitor_clear();
	init_descriptor_tables();
	init_interupts();
	monitor_clear();

   initialise_paging();
   //sleep(500);
   
	monitor_write("#------------------------------------------------------------------------------#");
	monitor_write("| Welcome To MatrixOS                                                          |");
	monitor_write("#------------------------------------------------------------------------------#");
	monitor_write("> ");

	u32int *ptr = (u32int*)0xA0000000;
    u32int do_page_fault = *ptr;

	return 0;
}
