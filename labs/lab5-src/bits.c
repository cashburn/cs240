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
	/*int count = 0;
	for (int i = 0; i < 32; i++) {
		if (getBitAt(bitmap, i) == bitValue)
			count++;
	}
	return count;*/

	int count = 0;
	for (int i = 0; i < 32; i++) {
		if (bitmap & 1)
			count++;
		bitmap = bitmap >> 1;
	}
	return count;
}

// It returns the number of largest consecutive 1s in "bitmap".
// Set "*position" to the beginning bit index of the sequence.
int maxContinuousOnes(unsigned int bitmap, int * position) {
	int in = -1;
	int out = 0;
	int count;
	for (int i = 0; i < 32; i++) {
		if ((getBitAt(bitmap, i) == 1)) {
			if (in < 0) {
				in = i;
			}
			continue;
		}
		else {
			if (in >= 0) {
				out = i;
				if ((out - in) > count) {
					*position = in;
					count = out - in;
				}
				in = -1;
			}
		}
	}
	return count;
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

unsigned int replaceBits (unsigned int val, unsigned int from, unsigned int to, int n) {
	int count = 0;
	unsigned int newVal = 0;

	printBits(from);
	printBits(to);
	printf("\n");
	for (int i = 0; i < 32; i++) {
		while ((from&1) == (val&1)) {
			from = from >> 1;
			val = val >> 1;
			count++;
			if (count == n) {

				val = val << count;
				from = from << count;
				count = 0;
				printBits(val);
				
				printBits(from);
				printBits(newVal);

				printf("\n");
				for (int i = 0; i < n; i++) {
					newVal = newVal | (to & 1);
					newVal = newVal >> 1;
					to >> 1;
				}
				to << n;
			}
		}
		val >> 1;
		newVal = newVal | (to & 1);
		newVal = newVal >> 1;
	}
	//newVal << 1;
	return newVal;
}




