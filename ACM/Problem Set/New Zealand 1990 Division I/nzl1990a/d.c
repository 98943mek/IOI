#include <stdio.h>

int i,j,k,n;
int grid[1000][1000];

void clear() {
 int i,j;
 for (i=0; i<n; i++)
  for (j=0; j<n; j++)
   grid[i][j]=0;
 }

void dump() {
 int i,j;
 for (i=0; i<n; i++) {
  for (j=0; j<n; j++)
   if (grid[i][j])
    printf("%d ",j+1);
  printf("\n");
  }
 }

void mark(int i,int j) {
 grid[i-1][j-1]=1;
 }

void frame(int k) {
 int s,t,u,v,w;
 mark(1,1);
 for (s=1; s<=k; s++)
  for (t=(s-1)*(k-1)+2;t<=s*(k-1)+1;t++) {
   mark(s,t); mark(t,s); }
 for (u=1; u<=k-1; u++)
  for (v=1; v<=k-1; v++) {
   s=u*(k-1)+2;
   t=v*(k-1)+2;
   for (w=0; w<=k-2; w++)
    mark(s+(w+(u-1)*(v-1))%(k-1),t+w);
   }
 }

int main() {
 scanf("%d",&k);
 while (k) {
  n=k*k-k+1;
  clear();
  frame(k);
  dump();
  scanf("%d",&k);
  }
 return 0;
 }
