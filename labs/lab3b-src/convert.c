#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
char final[32];
void strrev(char * str) {
	char * temp = str;
	while (temp && *temp)
		temp++;
	temp--;
	while (str < temp) {
		*str = *str ^ *temp;
		*temp = *str ^ *temp;
		*str = *str ^ *temp;
		str++;
		temp--;
	}
}
int to10(int basefrom, char * number) {
	int current = 0;
	int final = 0;
	int i = strlen(number) - 1;
	while (*number) {
		if ((*number >= 'A') && (*number <= 'O')) 
			current = (*number - 'A') + 10;
		else if ((*number >= '0') && (*number <= '9'))
			current = *number - '0';
		double temp = pow((double) basefrom, (double) i);
		final += (current * (int) temp);
		number++;
		i--;
	}
	printf("Converted to base 10: %d\n", final);
	return final;
}

char * from10(int baseto, int number) {
	int remain;
	while (number) {
		remain = (number % baseto);
		if (remain > 9)
			sprintf(final + strlen(final), "%c", remain + 'A' - 10);
		else
			sprintf(final + strlen(final), "%d", remain);
		number = (number / baseto);
	}
	strrev(final);
	return final;
}
int main(int argc, char ** argv) {
	char * number;
	if (argc != 4) {
		printf("Usage:  convert <basefrom> <baseto> <number>\n");
		exit(1);
	}
	int basefrom = atoi(argv[1]);
	int baseto = atoi(argv[2]);
	strcpy(number, argv[3]);
	printf("Number read in base %d: %s\n", basefrom, number);
	char * result = from10(baseto, to10(basefrom, number));
	printf("Converted to base %d: %s\n", baseto, result);
}