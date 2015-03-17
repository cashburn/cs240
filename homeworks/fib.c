#include <stdio.h>
int main() {
	int done = 0;
	while (!done) {
		printf("Program Fibonacci\n");
		int k;
		printf("Input k? ");
		scanf("%d", &k);
		getchar();
		printf("The %dth Fibonacci number is %d\n",k,fib(k));
		printf("Do you want to continue y/n? ");
		int c;
		int exit = 0;
		while (!exit) {
			c = getchar();
			printf("%d\n",c);
			getchar();
			if(c == 'y') {
				exit = 1;
				continue;
			}
			else if (c == 'n') {
				done = 1;
				exit = 1;
			}
			else
				printf("type y or n");
		}
	}
	printf("Bye!\n");
}
int fib(int n) {
	if (n == 0)
		return 0;
	if (n == 1)
		return 1;
	return fib(n-1) + fib(n-2);
}
