#include "monitor.h"
#include "common.h"

int main(struct multiboot *mboot_ptr) {
	monitor_clear();
	monitor_write("#------------------------------------------------------------------------------#");
	monitor_write("| Welcome To MatrixOS                                                          |");
	monitor_write("#------------------------------------------------------------------------------#");
	monitor_write("> ");
	return 0;
}
