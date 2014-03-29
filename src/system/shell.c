#include "shell.h"
#include "../stdio.h"

#define BUF_SIZE_INIT 64

char *input;
u32int size_input = 0;
u8int running = 0;
u8int shell_running = 0;

void process ( char* str );

/*
//Returns positive if failed(no space left)
*/
int resizebuf ( void* ptr, int size ) {
	ptr = ( void* ) krealloc ( ptr, size_input, size );
	size_input = size_input + BUF_SIZE_INIT;
	printf ( "resized" );
}

void startShell() {
	syscall_monitor_write ( "MatrixShell v1.0\n" );
	input = ( char* ) kmalloc ( BUF_SIZE_INIT*sizeof ( char ) );
	size_input = BUF_SIZE_INIT;
	shell_running = 1;
	running = 1;
	doShell();
}

void doShell() {
	while ( shell_running ) {
		syscall_monitor_write ( "> " );
		int i = 0;

		while ( running ) {
			char tmp = getKey();

			if ( tmp != 0 ) {
				input[i] = tmp;
				i++;

				if ( size_input == i ) {
					printf ( "resizeing..." );
					resizebuf ( input, size_input = BUF_SIZE_INIT*sizeof ( char ) );
				}

				if ( tmp == 0xd ) {
					break;
				}

				monitor_put ( tmp );
			}
		}

		process ( input ); //lets do this shit :)
	}
}

void help() {
	monitor_set_back_colour ( 10 );
	monitor_set_fore_colour ( 0 );
	printf ( "This is MatrixOS HELP!\n" );
	printf ( "Commands:\n" );
	printf ( "help - displays this text\n" );
	printf ( "info - infomation about MatixOS\n" );
	printf ( "exit - quit this terminal session\n" );
	monitor_set_fore_colour ( 15 );
	monitor_set_back_colour ( 0 );
}

void info() {
	printf ( "Welcome to MatrixOS " );
	print_version();
	printf ( "\nMatrixOS is written by Grant Perry and is a learning\nproject\
	that he started writing in the early\nmonths of 2014.\n" );
}

/*
// Checks lenght and chars to see if str1 == str2/
// Returns -1 if length unequal
// Returns true or 1 if equal
*/
u8int checkstr ( char* str1, char* str2 ) {
	u32int i = 0;
	u32int cor = 0;
	u32int large = 0;

	if ( strlen ( str1 ) <= strlen ( str2 ) ) {
		large = strlen ( str2 );
	}

	if ( strlen ( str1 ) >= strlen ( str2 ) ) {
		large = strlen ( str1 );
	}

	while ( ( large-1 ) >= i ) {
		if ( str1[i] != str2[i] ) {
			return 0;

		} else {
			cor++;
		}

		i++;
	}

	return cor;
}

typedef struct buf {
	u32int start;
	u32int end;
} buf_t;

void process ( char* str ) {
	buf_t list[64];
	u32int     x=0,  y=0, spaces = 0, chars = 0, c = 0, i = 0;
	u32int lastspace = -1, lastchar = -1;

	while ( c <= sizeof ( str ) ) {
		if ( str[c] == ' ' && ( lastchar == c-1 ) || str[c] == '\r' ) {
			list[i].end = c - 1;
			i++;
			lastspace = c;
		}

		if ( str[c] != ' ' && ( lastspace == c-1 ) ) {
			lastchar = c;
			list[i].start = c;
		}

		c++;
	}


	char command[32] = {};
	u32int p = 0;

	while ( p <= 31 ) {
		command[p] = 0;
		p++;
	}

	u32int t = 0;
	u32int len = list[0].end - list[0].start;

	while ( t <= len ) {
		command[t] = str[list[0].start + t];
		t++;
	}

	if ( checkstr ( command, "help" ) ) {
		help();
		return;
	}

	if ( checkstr ( command, "info" ) ) {
		info();
		return;
	}

	if ( checkstr ( command, "exit" ) ) {
		shell_running = 0;
		return;
	}

	printf ( "\ncommand not recognised" );
}
