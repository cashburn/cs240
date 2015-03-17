#include <stdio.h>
#include <string.h>
#include <stdlib.h>
char * strrev(char * string) {
	char * s = string;
	char * new = (char *) malloc(sizeof(string));
	char * new1 = new;
	while (*s) {
		new1++;
		s++;
	}
	*new1 = 0;
	new1--;
	s = string;
	while (*s) {
		*new1 = *s;
		new1--;
		s++;
	}
	return new;
}

char * convert (int base, int n) {
	char * final = (char *) malloc(32 * sizeof(char));
	while (n) {
		int remain = n % base;
		if (remain > 9)
			sprintf(final + strlen(final), "%c", remain + 'a' -10);
		else
			sprintf(final + strlen(final), "%d", remain);
		n = n / base;
	}
	return strrev(final);
}

int main(int argc, char**argv) {
	if (argc != 2) {
		printf("Usage: tohexbin arg1");
		exit(1);
	}
	int input;
	if (sscanf(argv[1], "%d", &input) == EOF) {
		printf("arg1 must be an integer");
		exit(1);
	}
	printf("Binary: 0b%s\n", convert(2, input));
	printf("Hexadecimal: 0x%s\n", convert(16, input));
}

