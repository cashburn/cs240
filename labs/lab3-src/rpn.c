
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
		if ((c == ' ') || (c == '\n'))
			continue;
		if ((c >= '0') && (c <= '9')) {
			char temp[5];
			fseek(fd, -1l, SEEK_CUR);
			fscanf(fd, "%s", temp);
			stack_push(atof(temp));
			while (((c = fgetc(fd)) != '\n') && (c != ' ')) { }
			count++;
		}
		if (c == '+') {
			stack_push(add(stack_pop(), stack_pop()));
			count--;
		}
		if (c == '-') {
			stack_push(sub(stack_pop(), stack_pop()));
			count--;
		}
		if (c == '/') {
			stack_push(div(stack_pop(), stack_pop()));
			count--;
		}
		if (c == '*') {
			stack_push(mul(stack_pop(), stack_pop()));
			count--;
		}
		if (c == 'x') {
			stack_push(x);
			count++;
		}
		if (c == 's') {
			if ((c = fgetc(fd)) == 'i')
				if ((c = fgetc(fd)) == 'n') {
					stack_push(sin(stack_pop()));
					count++;
				}
		}
		if (c == 'c') {
			if ((c = fgetc(fd)) == 'o')
				if ((c = fgetc(fd)) == 's') {
					stack_push(cos(stack_pop()));
					count++;
				}
		}
		if (c == 'e') {
			if ((c = fgetc(fd)) == 'x')
				if ((c = fgetc(fd)) == 'p') {
					stack_push(exp(stack_pop()));
					count++;
				}
		}
		if (c == 'p') {
			if ((c = fgetc(fd)) == 'o')
				if ((c = fgetc(fd)) == 'w') {
					stack_push(pow(stack_pop(),stack_pop()));
					count++;
				}
		}
		if (c == 'l') {
			if ((c = fgetc(fd)) == 'o')
				if ((c = fgetc(fd)) == 'g') {
					stack_push(log(stack_pop()));
					count++;
				}
		}
	}
	if (count == 1)
		return stack_pop();
	
	
}

