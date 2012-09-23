#include<stdio.h>
#include<stdlib.h>
#define rnd(p) (random()%(p))
#define N 25000
int i,j,k;
int r,c,l,x,y,c1,c2;
int main() {
 for (i=0; i<N; i++) {
	r=1+rnd(10);
	c=1+rnd(10);
	x=1+rnd(r);
	y=1+rnd(c);
	c1=1+rnd(6);
	do c2=1+rnd(6); while ((c1+c2==7)||(c1==c2));
	l=1+rnd(20);
	for (j=0; j<l; j++)
		printf("%c",'A'+rnd(26));
	printf("\n");
	printf("%d %d %d %d %d %d\n",r,c,x,y,c1,c2);
	for (j=0; j<r; j++)
		for (k=0; k<c; k++) {
			c1=((j==x-1)&&(k==y-1))?1+rnd(6):-1+rnd(8);
			printf(k==c-1?"%d\n":"%d ",c1);
			}
	}
 printf("END\n");
 return 0;
 }
