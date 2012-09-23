#include<stdio.h>
#define CHYBA 1

FILE *vstup,*vystup;
int grid[2048][2048];
int k,n;
int h,i,j,y;

void clear() {
 for (i=0; i<n; i++)
  for (j=0; j<n; j++)
   grid[i][j]=0;
 }

int main(int argc,char **argv) {
 vstup=fopen("../problems/d.in","r");
 vystup=fopen(argv[1],"r");
 fscanf(vstup,"%d",&k);
 while (k) {
  n=k*k-k+1;
  clear();
  for (i=0; i<n; i++)
   for (j=0; j<k; j++) {
    fscanf(vystup,"%d",&y);
    if (grid[i][y-1]) {
     printf("Error - same numbers on line\n");
     return CHYBA;
     }
    grid[i][y-1]=1;
    }
  for (j=0; j<n; j++)
   for (h=0; h<n; h++)
    if (grid[j][h])
     for (i=h+1; i<n; i++)
      if (grid[j][i])
       for (y=j+1; y<n; y++)
	if ((grid[y][h])&&(grid[y][i])) {
	 printf("Error - %d - [%d,%d][%d,%d]\n",k,j,h,y,i);
	 return CHYBA;
	}
  fscanf(vstup,"%d",&k);
  }
 fclose(vstup);
 fclose(vystup);
 return 0;
}
