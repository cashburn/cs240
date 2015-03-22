
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct WordInfo {
	char * word;
	int count;
};

typedef struct WordInfo WordInfo;

int maxWords;
int nWords;
WordInfo*  wordArray;

#define MAXWORD 100
int wordLength;
char word[MAXWORD];
FILE * fd;
int charCount;
int wordPos;

void toLower(char *s) {
	char * s1 = s;
	while (*s1) {
		if ((*s1 >= 'A') && (*s1 <= 'Z'))
			*s1 = *s1 - 'A' + 'a';
		if (*s1 == EOF)
			return;
		s1++;
	}
}

// It returns the next word from stdin.
// If there are no more more words it returns NULL. 
static char * nextword() {
	int c;
	char * string = malloc(MAXWORD * sizeof(char));
	char * string1 = string;
	while ((c = fgetc(fd)) != EOF) {
		charCount++;
		if (!((c >= 'a') && (c <= 'z')) && !((c >= 'A') && (c <= 'Z')))
			break;
		else
			*string1 = c;
		string1++;
	}
	if (c == EOF)
		return NULL;
	*string1 = 0;
	return string;
}

int
main(int argc, char **argv) {
	if (argc < 2) {
		printf("Usage: countwords filename\n");
		exit(1);
	}
	maxWords = 10;
	char * filename = argv[1];

	fd = fopen(filename, "r");
	wordArray = (WordInfo *) malloc(maxWords * sizeof(WordInfo));
	char * read;
	int temp;
	while ((read = nextword()) != NULL) {
		temp = 0;
		if (nWords == maxWords) {
			maxWords = 2 * maxWords;
			wordArray = (WordInfo *) realloc(wordArray, maxWords * sizeof(WordInfo));
		}
		if (strlen(read) == 0)
			continue;
		for (int i = 0; i < nWords; i++) {
			if (strcmp(wordArray[i].word, read) == 0) {
				wordArray[i].count++;
				temp = 1;
				break;
			}
		}
		if (temp)
			continue;
		toLower(read);
		wordArray[nWords].word = (char *) malloc(sizeof(read));
		strcpy(wordArray[nWords].word, read);
		wordArray[nWords++].count = 1;
	}
	int change = 1;
	for (int j = 0; j < nWords; j++) {
		//change = 0;
		for (int i = 0; i < nWords - 1; i++) {
			if (strcmp(wordArray[i].word, wordArray[i+1].word) > 0) {
				char * tempWord = strdup(wordArray[i].word);
				int tempCount = wordArray[i].count;
				wordArray[i].word = strdup(wordArray[i+1].word);
				wordArray[i].count = wordArray[i+1].count;
				wordArray[i+1].word = strdup(tempWord);
				wordArray[i+1].count = tempCount;
				//change = 1;
			}
		}
	}
	for (int i = 0; i < nWords; i++) {
		printf("%s %d\n", wordArray[i].word, wordArray[i].count);
	}
}

