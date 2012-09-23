#include<stdio.h>
#include<stdlib.h>
#define rnd(p) (random()%(p))
#define N 100000
int i,j;
int main() {
 for (i=0; i<N; i++)
	 if (rnd(50))
		 printf("%d %d\n",1+rnd(10000),1+rnd(10000));
	 else {
		 j=1+rnd(10000);
		 printf("%d %d\n",j,j);
	 	}
 printf("0 0\n");
 return 0;
 }
