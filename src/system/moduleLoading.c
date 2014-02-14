#include "moduleLoading.h"
#include "../common.h"
#include "../monitor.h"

void runModule(u8int (*func)(void)) {
	u8int x = monitor_getX();
	u8int y = monitor_getY();
	u8int fore_colour = monitor_get_fore_colour();
	monitor_set_cursor_pos(y, 0);
	monitor_write("[....] ");
	
	s8int ret = func(); //call the function
	
	if(ret == 0) {
		monitor_set_cursor_pos(y, 1);
		monitor_set_fore_colour(10);
		monitor_write("done");
	}
	if(ret >= 1) {
		monitor_set_cursor_pos(y, 1);
		monitor_set_fore_colour(13);
		monitor_write("warn");
	}
	if(ret <= -1) {
		monitor_set_cursor_pos(y, 1);
		monitor_set_fore_colour(4);
		monitor_write("eror");
	}
	monitor_set_fore_colour(fore_colour);
	monitor_set_cursor_pos(y+1, 0);
	
}
