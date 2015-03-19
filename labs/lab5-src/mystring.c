
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Adds src at the end of dest. Return the value of dest.
char *mystrcat(char * dest, char * src) {
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


// Return a pointer to the first occurrence of the substring needle in
// the string haystack. 
// Return NULL if the string is not found.
char * mystrstr(char * haystack, char * needle) {
	char * hay1 = haystack;
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


// Trims any spaces that are at the beginning or the end of s and returns s.
// Use the same memory passed in s. 
// Example:
// char s1[20];
// strcpy(s1,      hello    );
// strtrim(s1);
// printf(<s1=%s>\n, s1); // it will print <s1=hello>  without the spaces.
char * mystrtrim(char * s) {
	char * s1 = s;
	char * in;
	char * out;
	while ((*s1 == ' ')) {
		s1++;
	}
	in = s1;
	while ((*s1 != ' ') && *s1)
		s1++;
	out = s1;
	s1 = s;
	while (in != out) {
		*s1 = *in;
		s1++;
		in++;
	}
	*s1 = '\0';
	return s;
}


// Returns a new string that will substitute every occurrence of the substring from
//  to the substring to in the string src
// This call will allocate memory for the new string and it will return the new string.
// Example:
// char a[6];
// strcpy(a, apple);
// char * b = strreplace(a, pp, orange);
// printf(<b=%s>\n, b); // It will print <b=aorangele>  
char * mystrreplace( char * src, char * from, char * to)  {
	char * final;
	//do {
	final = (char *) malloc(sizeof(src)-sizeof(from)+sizeof(to));
	char * final1 = final;
	char * src1 = src;
	char * from1 = from;
	char * to1 = to;
	char * after;
		while (*src1) {
			if (!*from1) {
				after = src1;
				while (from1 != from) {
					from1--;
					src1--;
				}
				break;
			}
			if (*src1 == *from1)
				from1++;
			else {
				from1 = from;
				*final1 = *src1;
				final1++;
			}
			src1++;
		}
		if (!*src1) {
			return final;
		}
		while (*to1) {
			*final1 = *to1;
			final1++;
			to1++;
		}
		while (*after) {
			*final1 = *after;
			final1++;
			after++;
		}
		*final1 = '\0';
		/*final1 = final;
		src1 = final1;
		from1 = from;
		to1 = to;*/
	//} while (mystrstr(final, from) != NULL);
	return final;
}


