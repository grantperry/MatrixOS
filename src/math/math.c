#include <math.h>

int math_pow ( int base, int exponent ) {
	int x, output = 1;

	for ( x = 0; x < exponent; x++ ) {
		output = output * base;
	}

	return output;
}

int math_intLength ( int intNumber ) {
	int inv = 0, count = 0, originalInt = intNumber;

	if ( intNumber < 0 ) {
		intNumber = math_abs ( intNumber );
	}

	while ( intNumber > 0 ) {
		count = count + 1;

		inv = inv * 10 + ( intNumber % 10 );
		intNumber = intNumber / 10; //since reverseNumber is int, dividing by ten also floors integer
	}

	if ( originalInt != 0 ) {
		return count;

	} else if ( originalInt == 0 ) { //if the original in is 0, above will not enter while loop, so count will equal 0
		//and will return a number length of 0, so here i will brute return a length of 1
		return 1;
	}

}

float math_abs ( float absNumber ) {
	if ( absNumber < 0 ) {
		absNumber = -1 * absNumber;

		return absNumber;

	}
}

