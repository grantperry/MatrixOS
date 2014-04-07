// common.c -- Defines some global functions.
//			 From JamesM's kernel development tutorials.

#include "common.h"
#include "syscall.h"

u32int screen[25 * 80]; //char array to hold screen characters when k_save() is called.
extern u16int video_memory; // from monitor.c

/*
// print: Print an array of chars to std Text Mode via. Syscalls.
*/
int print ( char* c ) {
	syscall_monitor_write ( c );
}

/*
// Write a byte out to the specified port.
*/
void outb ( u16int port, u8int value ) {
	asm volatile ( "outb %1, %0" : : "dN" ( port ), "a" ( value ) );
}
/*
// Read Byte from specified port.
*/
u8int inb ( u16int port ) {
	u8int ret;
	asm volatile ( "inb %1, %0" : "=a" ( ret ) : "dN" ( port ) );
	return ret;
}

/*
// Read Word from specified port.
*/
u16int inw ( u16int port ) {
	u16int ret;
	asm volatile ( "inw %1, %0" : "=a" ( ret ) : "dN" ( port ) );
	return ret;
}

/*
// Copy len bytes from src to dest.
*/
void memcpy ( u8int *dest, const u8int *src, u32int len ) {
	const u8int *sp = ( const u8int * ) src;
	u8int *dp = ( u8int * ) dest;

	for ( ; len != 0; len-- ) {
		*dp++ = *sp++;
	}
}

/*
// Write len copies of val into dest.
*/
void memset ( u8int *dest, u8int val, u32int len ) {
	u8int *temp = ( u8int * ) dest;

	for ( ; len != 0; len-- ) {
		*temp++ = val;
	}
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
	if ( strlen ( str1 ) >= strlen ( str2 ) ) {
		large = strlen ( str1 );
	}

	if ( strlen ( str1 ) <= strlen ( str2 ) ) {
		large = strlen ( str2 );
	}

	while ( ( large-1 ) >= i ) {
		if ( str1[i] != str2[i] ) {
			return 0;

		} else {
			cor++;
		}

		i++;
	}
	if (strlen(str1) == strlen(str2)) {
		return cor;
	} else {
		return 0;
	}
}

/*
// Compare two strings. Should return -1 if
// str1 < str2, 0 if they are equal or 1 otherwise.
*/
int strcmp ( char *str1, char *str2 ) {
	int i = 0;
	int failed = 0;

	while ( str1[i] != '\0' && str2[i] != '\0' ) {
		if ( str1[i] != str2[i] ) {
			failed = 1;
			break;
		}

		i++;
	}

	// why did the loop exit?
	if ( ( str1[i] == '\0' && str2[i] != '\0' ) || ( str1[i] != '\0' && str2[i] == '\0' ) ) {
		failed = 1;
	}

	return failed;
}

/*
// Copy the NULL-terminated string src into dest, and
// return dest.
*/
char *strcpy ( char *dest, const char *src ) {
	do {
		*dest++ = *src++;
	} while ( *src != 0 );
}

/*
// Concatenate the NULL-terminated string src onto
// the end of dest, and return dest.
*/
char *strcat ( char *dest, const char *src ) {
	while ( *dest != 0 ) {
		*dest = *dest++;
	}

	do {
		*dest++ = *src++;
	} while ( *src != 0 );

	return dest;
}

/*
// Count the length of a char array.
*/
int strlen ( char *src ) {
	int i = 0;

	while ( *src++ ) {
		i++;
	}

	return i;
}

/*
// You know... Just complain about the bully and hide in a corner.
*/
extern void panic ( const char *message, const char *file, u32int line ) {
	// We encountered a massive problem and have to stop.
	asm volatile ( "cli" ); // Disable interrupts.

	printf( "PANIC(%s) at %s:%d\n", message, file, line );

	// Halt by going into an infinite loop.
	for ( ;; );
}

/*
//About the same as Panic.
*/
extern void panic_assert ( const char *file, u32int line, const char *desc ) {
	// An assertion failed, and we have to panic.
	asm volatile ( "cli" ); // Disable interrupts.

	printf ( "ASSERTION-FAILED(%s) at %s:%d\n", desc, file, line );

	// Halt by going into an infinite loop.
	for ( ;; );
}

void numToAsciChar ( char *asciInChar, int integerLength ) {
	int x;

	for ( x = 0; x < integerLength; x++ ) {
		asciInChar[x] = asciInChar[x] + 48; //(only works on integers)
	}

}

void intToChar ( int integer, char *numbersInChar ) {
	s32int integerLength = math_intLength ( integer ), x, endX;
	s32int number = integer % 10;

	//~ char *numbersInChar[integerLength];

	for ( x = 0; x < integerLength + 1; x++ ) {
		if ( x != 0 ) {
			integer = ( integer - number ) / 10;
			number = ( integer ) % 10;
		}

		endX = integerLength - x - 1;

		numbersInChar[endX] = number;
	}

}

void k_save() {
	u32int i;

	for ( i = 0; i < 25 * 80 + 1; i++ ) {
		screen[i] = video_memory + i;
	}

}
