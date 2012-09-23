#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define rnd(p) (random()%(p))
#define N 5000
#define USERS 1000
int i,j,k,l;
char c;
char users[USERS][32];
int exist[USERS];
char MTA[16];

int main() {
 for (i=0; i<USERS; i++) {
	k=1+rnd(USERS-i);
	l=5+rnd(11);
	for (j=0; j<l; j++) MTA[j]='a'+rnd(26);
	MTA[j]=0;
	printf("MTA %s %d",MTA,k-1);
	while (k--) {
		l=5+rnd(11);
		for (j=0; j<l; j++) users[i][j]='a'+rnd(26);
		users[i][j]=0;
		if (k) {
		  printf(" %s",users[i]);
		  exist[i]=1;
		  }
		else
		  exist[i]=0;
		strcpy(&(users[i][strlen(users[i])+1]),MTA);
		users[i][strlen(users[i])]='@';
		i++;
		}
	printf("\n");
 	}
 printf("*\n");
 for (i=0; i<N; i++) {
        do j=rnd(USERS); while (!exist[j]);
	printf("%s",users[j]);
	do
		printf(" %s",users[rnd(USERS)]);
	while (rnd(3));
	printf("\n*\n");
	while (rnd(5)) {
		for (k=rnd(73); k; k--) {
			do c='A'+rnd(26); while (c=='*');
			printf("%c",c);
			}
		printf("\n");
		}
 	printf("*\n");
	}
 printf("*\n");
 return 0;
 }
