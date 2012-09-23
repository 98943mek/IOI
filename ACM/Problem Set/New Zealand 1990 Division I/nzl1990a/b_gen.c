#define randomize() srand(4)
#define random(k) (rand()%k)
#include<stdio.h>
#include<stdlib.h>
#define LINES 7000

int i;

int main(){
 randomize();
 for (i=0; i<LINES; i++)
  printf("%d %d %d\n",random(19)+1,random(50)+1,random(50)+1);
 printf("0 0 0\n");
 return 0;
}
