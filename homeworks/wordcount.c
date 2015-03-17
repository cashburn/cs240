#include <stdio.h>
#include <string.h>
#define MAXSTRING 32
#define MAXWORDS 32

char ** array;
int counts[MAXWORDS];
int end = 0;
void array_add(char * s) {
	array[end] = (char *) malloc(strlen(s) + 1);
	strcpy(array[end], s);
	end++;
}
int search(char *s) {
	for (int i = 0; i < end; i++) {
		if (0 == strcmp(s, array[i]))
			return i;
	}
	array_add(s);
	return -1;
}
int main() {
	array = (char **) calloc (MAXSTRING, MAXWORDS * sizeof(char));
	int c;
	char * string = (char *) calloc(1,MAXSTRING * sizeof(char));
	char * s = string;
	while((c = getchar()) !='\n') {
		if((c >= 'A') && (c <= 'Z'))
			*s = c - 'A' + 'a';
		if((c >= 'a') && (c <= 'z'))
			*s = c;
		else {
			*s = 0;
			int i = search(string);
			if (i != -1)
				counts[i] = counts[i] + 1;
			else
				counts[end] = 1;
			s = string -1;
		}
		s++;
	}
	printf("Output: \n\n");
	printf("Histogram\n");
	printf("=================\n");
	for (int i = 0; i< end; i++) {
		printf("%s %d\n", array[i], counts[i]);
	}
}