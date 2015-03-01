
#include <stdio.h>
#include <float.h>
#include "array.h"

// Return sum of the array
double sumArray(int n, double * array) {
    double sum = 0;
  
    double * p = array;
    double * pend = p+n;

    while (p < pend) {
        sum += *p;
        p++;
    }

    return sum;
}

// Return maximum element of array
double maxArray(int n, double * array) {
    double * p = array;
    double * pend = p+n;
    double max = -DBL_MAX;

    while (p < pend) {
        if (*p > max)
            max = *p;
        p++;
    }
    return max;
}

// Return minimum element of array
double minArray(int n, double * array) {
    double * p = array;
    double * pend = p+n;
    double min = DBL_MAX;

    while (p < pend) {
        if (*p < min)
            min = *p;
        p++;
    }
  return min;
}

// Find the position in the array of the first element x 
// such that min<=x<=max or -1 if no element was found
int findArray(int n, double * array, double min, double max) {
    double * p = array;
    double * pend = p+n;
    int i = 0;

    while (p < pend) {
        if (*p >= min && *p <= max)
            return i;
        p++;
        i++;
    }
}

// Sort array without using [] operator. Use pointers 
// Hint: Use a pointer to the current and another to the next element
int sortArray(int n, double * array) {
	double * end = array + n;
	double * current = array;
	double * next = array + 1;
	double * next1 = array + 1;
	double temp;
	while (next1 < end) {
		while (next < end) {
			if (*current > *next) {
				temp = *current;
				*current = *next;
				*next = temp;
			}
			next++;
			current++;
		}
		current = array;
		next = array + 1;
		next1++;
	}
	return 0;
}

// Print array
void printArray(int n, double * array) {
	int i = 0;
	double * p = array;
	double * pend = p + n;
	while (p < pend) {
		printf("%d:%6f\n", i, *p);
		p++;
		i++;
	}
}

