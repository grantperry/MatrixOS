#include "shell.h"
#include "../stdio.h"

#define BUF_SIZE_INIT 64

char *input;
u32int size_input = 0;
u8int running = 0;

/*
//Returns positive if failed(no space left)
*/
int resizebuf(void* ptr, int size) {
	ptr = (void*)krealloc(ptr, size_input, size);
	size_input = size_input + BUF_SIZE_INIT;
	printf("resized");
}

void startShell() {
	syscall_monitor_write ( "MatrixShell v1.0\n" );
	input = (char*)kmalloc(BUF_SIZE_INIT*sizeof(char));
	size_input = BUF_SIZE_INIT;
	running = 1;
	doShell();
}

void doShell() {
	syscall_monitor_write ( "> " );
	int i = 0;
	while (running){
		char tmp = getKey();
		if (tmp != 0) {
			input[i] = tmp;
			i++;
			if(size_input == i) {
			printf("resizeing...");
				resizebuf(input, size_input = BUF_SIZE_INIT*sizeof(char));
			}
			if(tmp == 'p') {
				break;
			}
			monitor_put(tmp);
		}
	}
	syscall_monitor_write(input);
}
