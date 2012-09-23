#define randomize() srand(4)
#define random(k) (rand()%k)
#include<stdio.h>
#include<stdlib.h>
#define MAX 1600
#define LINES 16000

int i;

int main() {
 randomize();
 for (i=0; i<LINES; i++)
  printf("%d\n",random(MAX)+1);
 printf("0\n");
 return 0;
}
