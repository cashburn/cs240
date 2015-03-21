
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSTACK 50

char * stack;
int top;

void stack_push(char s) {
	stack[top++] = s;
}

char stack_pop() {
	char t = stack[--top];
	stack[top] = 0;
	return t;
}

int main(int argc,  char ** argv) {
	top = 0;
	stack = (char *) malloc(MAXSTACK * sizeof(char));
	if (argc != 2) {
		printf("Usage: isbalanced arg1");
		exit(1);
	}
	char * string = strdup(argv[1]);
	char * s = string;
	while (*s) {
		if ((*s == '{')||(*s == '(') || (*s == '['))
			stack_push(*s);
			else if(*s == '}') {
				char t = stack_pop();
				if (t != '{')
					stack_push('e');
			}
			else if(*s == ')') {
				char t = stack_pop();
				if (t != '(')
					stack_push('e');
			}
			else if(*s == ']') {
				char t = stack_pop();
				if (t != '[')
					stack_push('e');
			}
			s++;
	}
	if (top)
		printf("String is not balanced\n");
	else
		printf("String is balanced\n");
}

