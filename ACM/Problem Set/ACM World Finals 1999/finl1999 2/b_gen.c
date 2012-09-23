#include<stdio.h>
#include<stdlib.h>
#define rnd(p) (random()%(p))
#define N 1000
int i,j;
int n,d,x1,y1,z1,x2,y2,z2;
int main() {
 for (i=0; i<N; i++) {
	do {
		n=1+rnd(50);
		d=1+rnd(100);
		x1=-100+rnd(201);
		y1=-100+rnd(201);
		z1=-100+rnd(201);
		x2=-100+rnd(201);
		y2=-100+rnd(201);
		z2=-100+rnd(201);
		}
	while ((x1==x2)&&(y1==y2)&&(z1==z2));
	printf("%2d%5d%5d%5d%5d%5d%5d%5d\n",n,d,x1,y1,z1,x2,y2,z2);
	}
 printf(" 0\n");
 return 0;
 }
