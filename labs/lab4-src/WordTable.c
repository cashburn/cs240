#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "WordTable.h"

// Initializes a word table
void wtable_init(WordTable * wtable)
{
    // Allocate and initialize space for the table
    wtable->nWords = 0;
    wtable->maxWords = 10;
    wtable->wordArray = (WordInfo *) malloc(wtable->maxWords * sizeof(WordInfo));
    for (int i = 0; i < wtable->maxWords; i++) {
        llist_init(&wtable->wordArray[i].positions);
    }
}

// Add word to the table and position. Position is added to the corresponding linked list.
void wtable_add(WordTable * wtable, char * word, int position)
{
    // Find first word if it exists
    for (int i = 0; i < wtable->nWords; i++) {
        if (strcmp(wtable->wordArray[i].word, word) == 0 ) {
            // Found word. Add position in the list of positions
            llist_insert_last(&wtable->wordArray[i].positions, position);
            return;
        }
    }

    // Word not found.

    // Make sure that the array has space.
    // Expand the wordArray here.
    if (wtable->maxWords == wtable->nWords) {
        wtable->maxWords = (2 * (wtable->maxWords));
        wtable->wordArray = (WordInfo *) realloc(wtable->wordArray, wtable->maxWords * sizeof(WordInfo));
        for (int i = wtable->nWords; i < wtable->maxWords; i++)
            llist_init(&wtable->wordArray[i].positions);
    }
    // Add new word and position
    wtable->wordArray[wtable->nWords].word = strdup(word);
    llist_insert_last(&wtable->wordArray[wtable->nWords].positions, position);
    wtable->nWords++;
}

// Print contents of the table.
void wtable_print(WordTable * wtable, FILE * fd)
{
    fprintf(fd, "------- WORD TABLE -------\n");

    // Print words
    for (int i = 0; i < wtable->nWords; i++) {
        fprintf(fd, "%d: %s: ", i, wtable->wordArray[i].word);
        llist_print( &wtable->wordArray[i].positions);
    }
}

// Get positions where the word occurs
LinkedList * wtable_getPositions(WordTable * wtable, char * word) {
    for (int i = 0; i < wtable->nWords; i++) {
        if (strcmp(wtable->wordArray[i].word, word) == 0 )
            return &wtable->wordArray[i].positions;
    }
    return NULL;
}

//
// Separates the string into words
//

#define MAXWORD 200
char word[MAXWORD];
int wordLength;
int wordCount;
int charCount;
int wordPos;

// It returns the next word from stdin.
// If there are no more more words it returns NULL.
// A word is a sequence of alphabetical characters.
static char * nextword(FILE * fd) {
	char * nextword = malloc(MAXWORD * sizeof(char));
	char * i = nextword;
	int c;
	while ((((c = fgetc(fd)) >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z'))) {
		if (c == EOF)
			return NULL;
		*i = c;
		i++;
	}
	i = 0;
	return nextword;
}

// Conver string to lower case
void toLower(char * s) {
    char * i = s;
    //char * new = calloc(1,sizeof(s));
    //char * j = new;
    //int count = 0;
    while(*i) {
        if ((*i >= 'A') && (*i <= 'Z')) {
            *i = ('a' - 'A') + *i;
        }
        i++;
    }
}


// Read a file and obtain words and positions of the words and save them in table.
int wtable_createFromFile(WordTable * wtable, char * fileName, int verbose) {
    FILE * fd = fopen(fileName, "r");
    if (fd == NULL) {
        //printf("Could not open file, %s\n", file_name);
        return 0;
    }
    char * read = (char *) malloc(MAXWORD * sizeof(char));
    char * s = read;
    int i = 0;
    int c;
    charCount = 0;
    int charCountBeg = 0;
    int inWord = 0;
    while ((c = fgetc(fd)) != EOF) {
        charCount++;
        if (!((c >= 'A') && (c <= 'Z')) && !((c >= 'a') && (c <= 'z'))) {
            if (!inWord)
                continue;
            *s = 0;
            toLower(read);
            wtable_add(wtable, read, charCountBeg-1);
            if(verbose)
                printf("%d: word=%s, pos=%d\n", i, read, charCountBeg-1);
            i++;
            inWord = 0;
            s = read;
        }
        else {
            *s = c;
            if (!inWord)
                charCountBeg = charCount;
            inWord = 1;
            s++;
        }
    }
    fclose(fd);
    return 0;
}


// Sort table in alphabetical order.
void wtable_sort(WordTable * wtable) {
    int change = 1;
    WordInfo temp;
    while (change) {
        change = 0;
        for (int i = 1; i < wtable->nWords; i++) {
            if (strcmp(wtable->wordArray[i - 1].word, wtable->wordArray[i].word) > 0) {
                temp = wtable->wordArray[i - 1];
                wtable->wordArray[i-1] = wtable->wordArray[i];
                wtable->wordArray[i] = temp;
                change = 1;
            }
        }
    }
}

// Print all segments of text in fileName that contain word.
// at most 200 character. Use fseek to position file pointer.
// Type "man fseek" for more info.
int wtable_textSegments(WordTable * wtable, char * word, char * fileName) {
    int value = -1;
    FILE * fd = fopen(fileName, "r");
    if (fd == NULL)
        return 1;
    printf("===== Segments for word \"%s\" in book \"%s\" =====\n", word, fileName);
    for (int i = 0; i < wtable->nWords; i++) {
        if (strcmp(wtable->wordArray[i].word, word) == 0)
            value = i;
    }
    if (value == -1)
        return 1;
    ListNode * e = (wtable->wordArray[value].positions).head;
    while (e != NULL) {
        if (fseek(fd, e->value, SEEK_SET))
            return 1;
        printf("---------- pos=%d-----\n......", e->value);
        for (int i = 0; i < 200; i++) {
            char c = fgetc(fd);
            if (c == EOF)
                break;
            printf("%c", c);
        }
        printf("......\n");
        e = e->next;
    }
    return 0;        
}



