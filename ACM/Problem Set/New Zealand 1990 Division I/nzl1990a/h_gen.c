#define randomize() srand(4)
#define random(k) (rand()%(k))
#include<stdio.h>
#include<stdlib.h>

char vstup[50];
int i,j,k,l,m;

char *nazev() {
 int n=random(20)+2;
 int i;
 vstup[0]=random('Z'-'A'+1)+'A';
 for (i=1; i<n; i++) vstup[i]='a'+random('z'-'a'+1);
 vstup[i]=0;
 return vstup;
 }
char *cislo() {
 int i;
 switch(random(3)) {
 case 0: k=random(4)+4;
	 for (i=0; i<2; i++) vstup[i]='1'+random(9);
	 for (; i<k; i++) vstup[i]='0'+random(10);
	 vstup[k]=0;
	 break;
 case 1: vstup[0]='0';
	 vstup[1]='1'+random(9);
	 k=(vstup[1]-'0')/2+1;
	 k+=1+4+random(4);
	 for (i=2; i<k; i++) vstup[i]='0'+random(10);
	 vstup[k]=0;
	 break;
 case 2: vstup[0]=vstup[1]='0';
	 vstup[2]='1'+random(9);
	 k=2+7+random(5);
	 for (i=3; i<k; i++) vstup[i]='0'+random(10);
	 vstup[k]=0;
	 break;
 }
 return vstup;
 }

int main() {
 randomize();
 for (i=1; i<=4; i++)
  printf("00%d %s$%d\n",i,nazev(),random(150)+1);
 for (; i<=7; i++)
  for (j=1; j<=9; j++)
   printf("00%d%d %s$%d\n",i,j,nazev(),random(150)+1);
 for (; i<=9; i++)
  for (j=1; j<=9; j++)
   for (k=1; k<=9; k++)
    printf("00%d%d%d %s$%d\n",i,j,k,nazev(),random(150)+1);
 for (i=1; i<=1; i++)
  printf("0%d %s$%d\n",i,nazev(),random(150)+1);
 for (; i<=3; i++)
  for (j=1; j<=9; j++)
   printf("0%d%d %s$%d\n",i,j,nazev(),random(150)+1);
 for (; i<=5; i++)
  for (j=1; j<=9; j++)
   for (k=1; k<=9; k++)
    printf("0%d%d%d %s$%d\n",i,j,k,nazev(),random(150)+1);
 for (; i<=7; i++)
  for (j=1; j<=9; j++)
   for (k=1; k<=9; k++)
    for (l=1; l<=9; l++)
     printf("0%d%d%d%d %s$%d\n",i,j,k,l,nazev(),random(150)+1);
 for (; i<=9; i++)
  for (j=1; j<=9; j++)
   for (k=1; k<=9; k++)
    for (l=1; l<=9; l++)
     for (m=1; m<=9; m++)
      printf("0%d%d%d%d%d %s$%d\n",i,j,k,l,m,nazev(),random(150)+1);
 printf("000000\n");
 for (i=0; i<100; i++)
  for (j=0; j<1000; j++)
   printf("%s %d\n",cislo(),random(150)+1);
 printf("#\n");
 return 0;
}
