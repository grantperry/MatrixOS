#include "fun.h"
#include "../monitor.h"

void colour_fun() {
	monitor_clear();
	u16int i = 0;
	u8int c = 0;
	monitor_set_back_colour ( c );

	while ( i <= ( 80*25-2 ) ) {
		monitor_put ( ' ' );
		monitor_set_back_colour ( c );

		if ( c == 15 ) {
			c = 0;
		}

		c++;
		i++;
	}
}
