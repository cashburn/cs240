
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>

#include "rpn.h"
#include "nextword.h"
#include "stack.h"

double rpn_eval(char * fileName, double x) {
	FILE * fd = fopen(fileName, "r");
	if (fd == NULL) {
		printf("Could not open file %s\n", fileName);
		exit(1);
	}
	int count = 0;
	int c;
	char * i;
	char string[6];
	double add(double arg2, double arg1) {
        return arg1 + arg2;
    }
    double sub(double arg2, double arg1) {
        return arg2 - arg1;
    }
    double div(double arg2, double arg1) {
        return arg2 / arg1;
    }
    double mul(double arg2, double arg1) {
        return arg1 * arg2;
    }
	while ((c = fgetc(fd)) != EOF) {
		fseek(fd, -1l, SEEK_CUR);
		fscanf(fd, "%s", string);
		i = strstr((char *) string, "123456789");
		if (i != NULL) {
			stack_push(atof(string));
			while (((c = fgetc(fd)) != '\n') && (c != ' ')) { }
			count++;
		}
		if (strcmp(string, "+") == 0) {
			stack_push(add(stack_pop(), stack_pop()));
			count--;
		}
		if (strcmp(string, "-") == 0) {
			stack_push(sub(stack_pop(), stack_pop()));
			count--;
		}
		if (strcmp(string, "/") == 0) {
			stack_push(div(stack_pop(), stack_pop()));
			count--;
		}
		if (strcmp(string, "*") == 0) {
			stack_push(mul(stack_pop(), stack_pop()));
			count--;
		}
		if (strcmp(string, "x") == 0) {
			stack_push(x);
			count++;
		}
		if (strcmp(string, "sin") == 0) {
			stack_push(sin(stack_pop()));
			count++;
		}
		if (strcmp(string, "cos") == 0) {
			stack_push(cos(stack_pop()));
			count++;
		}
		if (strcmp(string, "exp") == 0) {
			stack_push(exp(stack_pop()));
			count++;
		}
		if (strcmp(string, "pow") == 0) {
			stack_push(pow(stack_pop(),stack_pop()));
			count++;
		}
		if (strcmp(string, "log") == 0) {
			stack_push(log(stack_pop()));
			count++;
		}
	}
	if (count == 1)
		return stack_pop();
	
	
}

