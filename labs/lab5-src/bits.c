#include <stdio.h>
#include <stdlib.h>

// It returns the bit value (0 or 1) at bit i
int getBitAt( unsigned int bitmap, unsigned int i) {
	unsigned int mask = 1 << i;
	return ((bitmap & mask) >> i);
}

// Sets the ith bit in *bitmap with the value in bitValue (0 or 1)
void setBitAt( unsigned int *bitmap, int i, int bitValue ) {
	int old = getBitAt(*bitmap, i);
	unsigned int mask;
	if(old) {
		mask = ~(1 << i);
		*bitmap = (*bitmap & mask);
	}
	else {
		mask = 1 << i;
		*bitmap = (*bitmap | mask);
	}


}

// It returns the number of bits with a value "bitValue".
// if bitValue is 0, it returns the number of 0s. If bitValue is 1, it returns the number of 1s.
int countBits( unsigned int bitmap, unsigned int bitValue) {
	int count = 0;
	for (int i = 0; i < 32; i++) {
		if (getBitAt(bitmap, i) == bitValue)
			count++;
	}
	return count;
}

// It returns the number of largest consecutive 1s in "bitmap".
// Set "*position" to the beginning bit index of the sequence.
int maxContinuousOnes(unsigned int bitmap, int * position) {
	// Add your code here
}

// It prints the bits in bitmap as 0s and 1s.
void printBits(unsigned int bitmap) {
	for (int i = 31; i >= 0; i--) {
		printf("%d", getBitAt(bitmap, i));
	}
	printf("\n");
	for (int i = 31; i >= 0; i--)
		printf("%d", i%10);
	printf("\n");
}


