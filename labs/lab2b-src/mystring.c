#include <stdio.h>
#include <stdlib.h>
#include "mystring.h"

// Type "man string" to see what every function expects.

int mystrlen(char * s) {
	int len = 0;
	while (*s) {
		len++;
		s++;
	}
	return len;
}

char * mystrcpy(char * dest, char * src) {
	char * src1 = src;
	char * dest1 = dest;	
	while (*src1) {	
		*dest1 = *src1;
		src1++;
		dest1++;
	}
	*dest1 = '\0';
	return dest;
}

char * mystrcat(char * dest, char * src) {
	char * src1 = src;
	char * dest1 = dest;
	while (*dest1) 
		dest1++;
	while (*src1) {
		*dest1 = *src1;
		src1++;
		dest1++;
	}
	*dest1 = '\0';
	return dest;
}
int mystrcmp(char * s1, char * s2) {
	char * ss1 = s1;
	char * ss2 = s2;
	while (*ss1 || *ss2) {
		if (*ss1 > *ss2) 
			return 1;
		if (*ss1 < *ss2)
			return -1;
		if (*ss1 == *ss2) {
			ss1++;
			ss2++;
		}
	}
	return 0;
}

char * mystrstr(char * hay, char * needle) {
	char * hay1 = hay;
	char * needle1 = needle;
	char * value;
	while (*hay1) {
		if (!*needle1) {
			while (needle1 != needle) {
				needle1--;
				hay1--;
			}
			return hay1;
		}
		if (*hay1 == *needle1)
			needle1++;
		else
			needle1 = needle;
		hay1++;
	}
	return NULL;
}

char * mystrdup(char * s) {
	char * src1 = s;
	char * dest = (char *) malloc(sizeof(s));
	while (*src1) {
		src1++;
		dest++;
	}
	while (src1 != s) {	
		*dest = *src1;
		src1--;
		dest--;
	}
	*dest = *s;
	return dest;
}

char * mymemcpy(char * dest, char * src, int n) {
	char * src1 = src;
	char * dest1 = dest;
	int i = 0;
	while (i < n) {
		*dest1 = *src1;
		src1++;
		dest1++;
		i++;
	}
}

