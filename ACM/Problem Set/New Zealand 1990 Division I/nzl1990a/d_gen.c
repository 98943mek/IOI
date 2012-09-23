#define randomize() srand(4)
#define random(k) (rand()%k)
#include<stdio.h>
#include<stdlib.h>

int i;

int main() {
 randomize();
 for (i=0; i<20; i++)
 switch (random(10)) {
  case 0: printf("2\n"); break;
  case 1: printf("3\n"); break;
  case 2: printf("4\n"); break;
  case 3: printf("6\n"); break;
  case 4: printf("8\n"); break;
  case 5: printf("12\n"); break;	  
  case 6: printf("14\n"); break;	  
  case 7: printf("18\n"); break;	  
  case 8: printf("20\n"); break;	  
  case 9: printf("24\n"); break;	  
  }
 printf("0\n");
 return 0;
}
