#include <stdio.h>
#include <string.h>
#define MAXSTACK 32
char stack[MAXSTACK];
int top = 0;
void stack_push(char s) {
	stack[top++] = s;
}
char stack_pop() {
	char temp = stack[--top];
	stack[top] = 0;
	return temp;
}
int main(int argc, char ** argv) {
	if (argc != 2) {
	printf("Usage: isbalanced arg1");
	exit(1);
	}
	char * string = strdup(argv[1]);
	char * s = string;
	while (*s) {
		if (*s == '{' || *s == '(')
			stack_push(*s);
		else if(*s == '}' || *s == ')') {
			char n = stack_pop();
			if (((*s == '}')&&(n != '{')) || ((*s == ')') && (n != '('))) {
				stack_push('e');
				break;
			}
		}
		s++;
	}
	if(top == 0)
		printf("String Is Balanced");
	else
		printf("String Not Balanced");
}