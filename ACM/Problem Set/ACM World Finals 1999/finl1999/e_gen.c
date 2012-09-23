#include<stdio.h>
#include<stdlib.h>
#define rnd(p) (random()%(p))
#define N 5000
int i,j,k;
int m,n;
int main() {
 for (i=0; i<N; i++) {
	m=1+rnd(50);
	printf("%d\n",m);
	for (j=0; j<m; j++) {
		n=rnd(21);
		printf("%d",n);
		for (k=0; k<n; k++)
			printf(" %d",1+rnd(20));	
		printf("\n");
		}
	}
 printf("0\n");
 return 0;
 }
