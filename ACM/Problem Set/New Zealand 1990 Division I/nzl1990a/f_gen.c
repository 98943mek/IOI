#define randomize() srand(4)
#define random(k) (rand()%(k))
#include<stdio.h>
#include<stdlib.h>
#define ZADANI 2
/* pocet ZADANI je *16 */

int had1[10000],had2[10000];
int i,j,jj,bodu;

void had() {
 int k;
 had1[0]=1+random(2);
 had2[0]=1+random(2);
 had1[1]=1+random(2);
 had2[1]=98+random(2);
 bodu=2;
 for (k=10; k; k--) {
  if (k!=10) {
   had2[bodu++]=k*10-random(5);
   had1[bodu-1]=had1[bodu-2];
   } 
  for (; had1[bodu-1]<95; had1[bodu-1]=had1[bodu-2]+random(5)+1)
   had2[bodu++]=k*10-random(5);
  had2[bodu++]=k*10-random(5)-5;
  had1[bodu-1]=had1[bodu-2];
  for (; had1[bodu-1]>7; had1[bodu-1]=had1[bodu-2]-random(5)-1)
   had2[bodu++]=k*10-5-random(5);
  }
 }

int main() {
 randomize();
 for (i=0; i<ZADANI; i++)
  for (jj=0; jj<32; jj++)
   switch((jj&1) ? jj/8 : (jj/2)%4 ) {
   case 0:
     had();
     printf("%d",bodu);
     for (j=0; j<bodu; j++) printf(" %d %d",had1[j],had2[j]);
     printf("\n");
     break;
   case 1:
     had();
     printf("%d",bodu);
     for (j=0; j<bodu; j++) printf(" %d %d",101-had2[j],had1[j]);
     printf("\n");
     break;
   case 2:
     had();
     printf("%d",bodu);
     for (j=0; j<bodu; j++) printf(" %d %d",101-had1[j],101-had2[j]);
     printf("\n");
     break;
   case 3:
     had();
     printf("%d",bodu);
     for (j=0; j<bodu; j++) printf(" %d %d",had2[j],101-had1[j]);
     printf("\n");
     break;
   }
 printf("0\n");
 return 0;
 }
