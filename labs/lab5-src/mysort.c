#include "mysort.h"
#include <alloca.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

//
// Sort an array of element of any type
// it uses "compFunc" to sort the elements.
// The elements are sorted such as:
//
// if ascending != 0
//   compFunc( array[ i ], array[ i+1 ] ) <= 0
// else
//   compFunc( array[ i ], array[ i+1 ] ) >= 0
//
// See test_sort to see how to use mysort.
//
void mysort( int n,                      // Number of elements
	     int elementSize,            // Size of each element
	     void * array,               // Pointer to an array
	     int ascending,              // 0 -> descending; 1 -> ascending
	     CompareFunction compFunc )  // Comparison function.
{
	void * temp = malloc(elementSize);
	for (int i; i < n; i++) {
		for (int j; j < n; j++) {
			void * current = (char *) (array + (j * elementSize));
			void * next = (char *) (array + ((j+1) * elementSize));
			if (ascending) {
				if ((*compFunc) (current, next) > 0) {
					memcpy(temp, current, elementSize);
					memcpy(current, next, elementSize);
					memcpy(next, temp, elementSize);
				}
			}
			else {
				if ((*compFunc) (current, next) < 0) {
					memcpy(temp, current, elementSize);
					memcpy(current, next, elementSize);
					memcpy(next, temp, elementSize);
				}
			}
		}
	}
	free(temp);
}

