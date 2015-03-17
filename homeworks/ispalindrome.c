#include <stdio.h>
#include <string.h>

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
int isPalindrome(char * string) {
	char * reverse = strrev((string));
	char * s = string;

	while ((*s == *reverse) && *s && *reverse) {
		s++;
		reverse++;
	}
	if(!*s && !*reverse)
		return 1;
	else
		return 0;
}

char * toalpha(char * raw) {
	char * new = (char *) malloc(sizeof(raw));
	char * s = raw;
	char * t = new;
	while (*s) {
		if ((*s >= 'A') && (*s <= 'Z')) {
			*t = *s -'A' +'a';
			t++;
		}
		if((*s >= 'a') && (*s <= 'z')) {
			*t = *s;
			t++;
		}
		s++;
	}
	*t = 0;
	return new;
}

main(int argc, char ** argv) {
	if (argc != 2) {
		printf("Usage: isPalindrome arg1");
		exit(1);
	}
	char * raw = strdup(argv[1]);
	char * string = toalpha(raw);
	if (isPalindrome(string))
		printf("Yes\n");
	else
		printf("No\n");
}