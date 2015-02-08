
#include <stdio.h>
#include "stack.h"
#include <stdlib.h>

int top=0;
double stack[MAXSTACK];

void stack_clear() 
{
	int i = 0;
	while (stack[i])
		stack[i++] = 0;
	top = 0;
}

double stack_pop()
{
	double x = stack[--top];
	stack[top] = 0;
	return x;
}

void stack_push(double val)
{
	stack[top++] = val;
}

void stack_print()
{
	printf("Stack:\n");
	if (!top) 
		printf("Stack is empty");

	for (int i = 0; i < top; i++)
		printf("%d: %f\n", i, stack[i]);
		
}

int stack_top()
{
  return top;
}

int stack_max()
{
  return MAXSTACK;
}

int stack_is_empty()
{
  return top == 0;
}


