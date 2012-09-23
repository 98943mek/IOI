#include <stdio.h>
#include <string.h>
#undef DEBUG
#define MAX 16
#define Row(x) (((x)>>9)&15)
#define Col(x) (((x)>>5)&15)
#define Pos(x) ((x)&31)
#define Code(x,y,z) (((x)<<9)|((y)<<5)|(z))
int right(int,int);
int left(int f,int u) {
 if (f>3) return right(7-f,u);
 switch(f) {
  case 1:
   if (u==3) return 2;
   if (u==2) return 4;
   if (u==4) return 5;
   if (u==5) return 3;
   printf("ERROR!\n");
   exit(0);
  case 2:
   if (u==3) return 6;
   if (u==6) return 4;
   if (u==4) return 1;
   if (u==1) return 3;
   printf("ERROR!\n");
   exit(0);
  case 3:
   if (u==6) return 2;
   if (u==2) return 1;
   if (u==1) return 5;
   if (u==5) return 6;
   printf("ERROR!\n");
   exit(0);
  }
  printf("ERROR!\n");
  exit(0);
 }
int right(int f,int u) {
 if (f>3) return left(7-f,u);
 switch(f) {
  case 1:
   if (u==3) return 5;
   if (u==2) return 3;
   if (u==4) return 2;
   if (u==5) return 4;
   printf("ERROR!\n");
   exit(0);
  case 2:
   if (u==3) return 1;
   if (u==6) return 3;
   if (u==4) return 6;
   if (u==1) return 4;
   printf("ERROR!\n");
   exit(0);
  case 3:
   if (u==6) return 5;
   if (u==2) return 6;
   if (u==1) return 2;
   if (u==5) return 1;
   printf("ERROR!\n");
   exit(0);
  }
  printf("ERROR!\n");
  exit(0);
 }
int code(int u,int f) {
 if (f>u) f--;
 return (u-1)*5+(f-1)+1;
 }
void decode(int c,int *u,int *f) {
 c--;
 *u=c/5; *f=c%5;
 if (*f>=*u) (*f)++;
 (*u)++; (*f)++;
 }
char name[32];
int bludiste[MAX][MAX][32];
int mapa[MAX][MAX];
int fronta[MAX*MAX*32];
int fronta_;
int rows,cols,r0,c0,u0,f0;
void zarad(int cod,int u0,int r,int c,int u,int f) {
 int cub;
 if ((r==-1)||(c==-1)||(r==rows)||(c==cols)) return;
 cub=code(u,f);
 if (bludiste[r][c][cub]) return;
 if ((mapa[r][c]!=-1)&&(mapa[r][c]!=u0)) return;
 bludiste[r][c][cub]=cod;
 fronta[fronta_++]=Code(r,c,cub);
 }
void vypis(int cod,int *q) {
 int r=Row(cod);
 int c=Col(cod);
 int p=Pos(cod);
#ifdef DEBUG
 printf("VYPIS: %d %d %d\n",r,c,p);
#endif 
 if ((r==r0)&&(c==c0)) {
  printf("(%d,%d),",r+1,c+1);
  *q=1;
  return;
  }
 vypis(bludiste[r][c][p],q); 
 printf("(%d,%d),",r+1,c+1);
 *q=(*q+1)%9;
 if (!*q) printf ("\n  ");
 }
int bfsearch(int cod) {
 int r=Row(cod);
 int c=Col(cod);
 int u,f;
 int q;
 decode(Pos(cod),&u,&f);
#ifdef DEBUG
 printf("SEARCH: %d %d %d %d\n",r,c,u,f);
#endif 
 if ((r0==r)&&(c0==c)&&(fronta_>1)) {
  printf("%s\n  ",name);
  vypis(bludiste[r][c][Pos(cod)],&q);
  printf("(%d,%d)\n",r0+1,c0+1);
  return 1;
  }
 zarad(cod,u,r-1,c,f,7-u);
 zarad(cod,u,r+1,c,7-f,u);
 zarad(cod,u,r,c-1,left(f,u),f);
 zarad(cod,u,r,c+1,right(f,u),f);
 return 0;
 }
int i,j; 
int main() {
 scanf("%s",name);
 for (;strcmp(name,"END");) {
  scanf("%d %d %d %d %d %d",&rows,&cols,&r0,&c0,&u0,&f0);
  r0--; c0--;
  for (i=0; i<rows; i++)
   for (j=0; j<cols; j++)
    scanf("%d",&mapa[i][j]);
  fronta_=1;
  fronta[0]=Code(r0,c0,code(u0,f0));
  for (i=0; i<fronta_; i++)
    if (bfsearch(fronta[i]))
      break;
  if (i==fronta_) printf("%s\n  No Solution Possible\n",name);
  for (i=1; i<fronta_; i++)
  	bludiste[Row(fronta[i])][Col(fronta[i])][Pos(fronta[i])]=0;
  scanf("%s",name);
  }
 return 0;
}
