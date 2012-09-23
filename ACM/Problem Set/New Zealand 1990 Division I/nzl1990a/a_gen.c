#define randomize() srand(4)
#define random(k) (rand()%(k))
#include<stdio.h>
#include<stdlib.h>
#define LINES 500
#define MPOINTS 50

long i,j,k;
char nazev[30];
long bodx[MPOINTS],body[MPOINTS];

int main() {
 randomize();
 for (j=0; j<LINES; j++) {
  k=random(15)+5;
  for (i=0; i<k; i++) nazev[i]=random('Z'-'A'+1)+'A';
  nazev[k]=0;
  printf("%s\n",nazev);
  do k=random(MPOINTS); while (k<5);
  bodx[0]=1;
  for (i=1; i<k; i++) bodx[i]=bodx[i-1]+1+random(5);
  for (i=1; i<k; i++) body[i]=random(30)+1;
  i=1+random(k-3);
  printf("%ld %ld\n",bodx[i],body[i]+1);
  printf("%d %d ",1,random(50)+1);
  for (i=1; i<k; i++)
   printf("%ld %ld ",bodx[i],body[i]);
  for (i=k-1; i>0; i--)
   printf("%ld %ld ",bodx[i],body[i]+2+random(30));
  printf("0 0\n");
  }
 printf("#\n");
 return 0;
}



