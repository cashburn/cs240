#include <stdio.h>

int
main(int argc, char **argv) {
  int finished = 0;
  printf("Welcome to the High Low game...\n");
  while (!finished) {
  char c;
  printf("Think of a number between 1 and 100 and press press <enter>\n");
  scanf("%c",&c);
  int low = 1;
  int high = 100;
  int mid = (low+high)/2;
  
  while(high>=low) {
    printf("Is it higher than %d? (y/n)\n", mid);
    scanf("%c",&c);
    getchar();
    if (c == 'y') {
      low = mid + 1;
      mid = (low+high)/2;
      }
    else if (c == 'n') {
      high = mid - 1;
      mid = (low+high)/2;
      }
    else {
     printf("Type y or n\n");
     }
  }
  printf("\n>>>>>> The number is %d\n\n", low);
  printf("Do you want to continue playing (y/n)?");
  scanf("%c",&c);
  getchar();
  if (c == 'n') {
  finished = 1;
  printf("Thanks for playing!!!");
  }
  }
}

