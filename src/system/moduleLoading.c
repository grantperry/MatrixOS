#include "moduleLoading.h"
#include "../common.h"
#include "../monitor.h"

extern u8int SYSCALLS_ENABLED;

void runModule(s8int (*func)(void)) {
	u8int x = monitor_getX();
	u8int y = monitor_getY();
	u8int fore_colour = monitor_get_fore_colour();
	monitor_set_cursor_pos(0, y);
	if (SYSCALLS_ENABLED) syscall_monitor_write("[....] ");
	if (!SYSCALLS_ENABLED) monitor_write("[....] ");
	
	s8int ret = func(); //call the function
	
	if(ret == 0) {
		monitor_set_cursor_pos(1, y);
		monitor_set_fore_colour(10);
		if (SYSCALLS_ENABLED) syscall_monitor_write("done");
		if (!SYSCALLS_ENABLED) monitor_write("done");
	}
	if(ret >= 1) {
		monitor_set_cursor_pos(1, y);
		monitor_set_fore_colour(13);
		if (SYSCALLS_ENABLED) syscall_monitor_write("warn");
		if (!SYSCALLS_ENABLED) monitor_write("warn");
	}
	if(ret <= -1) {
		monitor_set_cursor_pos(1, y);
		monitor_set_fore_colour(4);
		if (SYSCALLS_ENABLED) syscall_monitor_write("eror");
		if (!SYSCALLS_ENABLED) monitor_write("eror");
	}
	monitor_set_fore_colour(fore_colour);
	monitor_set_cursor_pos(0, y+1);
	
}
