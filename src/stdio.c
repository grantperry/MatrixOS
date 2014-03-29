


// stdio.h
#include "stdio.h"
#include "common.h"
#include "/usr/lib/gcc/i486-linux-gnu/4.6/include/stdarg.h"
#include "task.h"

extern volatile task_t *current_task;

void printf ( char *c, ... ) {
	va_list arguments;

	int i, numberOfArgs = 0, stringLength = strlen ( c );
	int integerArg;

	current_task->time_to_run + stringLength; // so that println is not interupted

	for ( i = 0; c[i]; i++ ) //checks number arguments is correct
		if ( c[i] == '%' ) {
			numberOfArgs++;
		}

	i = 0;

	va_start ( arguments, *c );

	while ( c[i] ) {

		if ( c[i] == '%' ) {
			if ( c[i+1] == 'd' ) {
				integerArg = va_arg ( arguments, int );

				int numberLength = math_intLength ( integerArg );
				int x;

				char integer[numberLength];

				intToChar ( integerArg, integer ); //sets integer as char

				numToAsciChar ( integer, numberLength ); //sets char to ASCII for printing

				for ( x = 0; x < numberLength; x++ ) {
					monitor_put ( integer[x] );

				}

				i = i + 2;

			} else if ( c[i+1] == 's' ) { //user wants to print a string
				char *charArrayArg;
				charArrayArg = va_arg ( arguments, char* );

				printf ( charArrayArg );
				i = i + 2;

			} else if ( c[i+1] == 'c' ) { //user wants to print a char
				char *charArg;
				charArg = va_arg ( arguments, char* );

				monitor_put ( charArg );
				i = i + 2;

			} else if ( c[i+1] == 'h' ) { //user wants to print a hex number

				u32int hexArg = va_arg ( arguments, u32int );

				s32int tmp;

				printf ( "0x" );

				char noZeroes = 1;

				int hexCount;

				for ( hexCount = 28; hexCount > 0; hexCount -= 4 ) {
					tmp = ( hexArg >> hexCount ) & 0xF;

					if ( !tmp && noZeroes ) {
						continue;
					}

					if ( tmp >= 0xA ) {
						noZeroes = 0;
						monitor_put ( tmp-0xA+'a' );

					} else {
						noZeroes = 0;
						monitor_put ( tmp+'0' );
					}
				}

				tmp = hexArg & 0xF;

				if ( tmp >= 0xA ) {
					monitor_put ( tmp - 0xA + 'a' );

				} else {
					monitor_put ( tmp+'0' );
				}

				i = i + 2;
			}
		}

		if ( i < stringLength + 1 && c[i] != '%' ) {
			if ( c[i] == 0 ) { //if i is at the terminating 0 '\000' of a string, break from loop
				break;

			} else {
				monitor_put ( c[i] );
				i++;
			}
		}
	}

	va_end ( arguments );
}



FILE* fopen ( fs_node_t* dir, char* file_name, char* mode ) {
	FILE* tmpf = find ( dir, file_name );

	if ( !tmpf ) {
		return 0;
	}

	if ( mode[0] == 'w' ) {
		tmpf->permissions.write = 1;
		tmpf->permissions.read = 0;

		return tmpf;
	}

	if ( mode[0] == 'r' ) {
		tmpf->permissions.write = 0;
		tmpf->permissions.read = 1;

		return tmpf;
	}


	return tmpf;
}

/*-----------------------------------------------------------------------------
 *
 *---------------------------------------------------------------------------*/
FILE* find ( fs_node_t* dir, char* file_name ) {
	int	i = 0;
	int idx = -1;

	for ( i = 0; i < dir->num_nodes; i++ ) {
		if ( !strcmp ( file_name, dir->ptr[i].name ) ) {
			idx = i;
			break;
		}
	}

	if ( idx == -1 ) {
		return 0;

	} else {
		return &dir->ptr[i];
	}
}

/*-----------------------------------------------------------------------------
 *
 *---------------------------------------------------------------------------*/
u32int fread ( FILE* file, void* buf, u32int size ) {
	u32int sz;// = read_fs ( ( fs_node_t* ) file, file->seek, size, ( u8int* ) buf );

	file->seek += sz;

	return sz;
}

/*-----------------------------------------------------------------------------
 *
 *---------------------------------------------------------------------------*/
void fseek ( FILE* file, u32int seek_pos ) {
	file->seek = seek_pos;
}


