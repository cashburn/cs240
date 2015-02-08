
#include <stdio.h>
#include <stdlib.h>

//
// Separates the file into words
//

#define MAXWORD 200
char word[MAXWORD];
int wordLength = 0;

// It returns the next word from fd.
// If there are no more more words it returns NULL. 
char * nextword(FILE * fd) {

	int c;
	int length = 0;
	while (word[length]) 
		word[length++] = 0;
	char * nextWord = (char *) &word;
	// While it is not EOF read char
		// While it is not EOF and it is a non-space char
		// store character in word.
		// if char is not in word return word so far.
		//
	length = 0;
	while ((c = fgetc(fd)) != EOF) {
		if ((c == ' ') || (c == '\t') || (c == '\n')) {
			if (length)
				break;
		}
		else {
			word[length] = (char) c;
			//nextWord = (char *) &c;
			length++;
		}
	}
	if (c == EOF)
		return NULL;
	// Return null if there are no more words
	return nextWord;
}

