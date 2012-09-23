#include<stdio.h>
#include<stdlib.h>
#define rnd(p) (random()%(p))
#define N 50000
int i,j;
int m;
int main() {
 for (i=0; i<N; i++) {
	m=1+rnd(10);
	printf("%d",m);
	for (j=0; j<m; j++) printf(" %.1f %.1f",rnd(251)/10.0,90*rnd(5)/4.0);
	printf("\n");	
	}
 printf("-2\n");
 return 0;
 }
