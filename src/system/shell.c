#include "shell.h"

void startShell() {
	syscall_monitor_write("MShell v1.0\n");
	doShell();
}

void doShell() {
	syscall_monitor_write("> ");
	//char input = keyboard_get();
	//syscall_monitor_write(input);
}
