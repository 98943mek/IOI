#include <stdio.h>

#define MAXWY 50
#define MAXB 20

int profit[MAXWY][MAXB];
int bnum[MAXWY];
int mnum[MAXWY];
int mval[MAXWY];
int mplace[MAXWY][MAXB];
char gotp[MAXWY*MAXB];
int WY;

void ReadInp(void)
{
  int i, j;

  scanf("%d", &WY);
  if (!WY)
    exit(0);
  for (i = 0; i < WY; i++) {
    scanf("%d", &bnum[i]);
    for (j = 0; j < bnum[i]; j++)
      scanf("%d", &profit[i][j]);
    profit[i][0] = 10 - profit[i][0];
    for (j = 1; j < bnum[i]; j++)
      profit[i][j] = 10 - profit[i][j] + profit[i][j-1];
  }
}

void GatherMax(void)
{
  int i, j;
  int max;

  for (i = 0; i < WY; i++) {
    max = 0;
    for (j = 0; j < bnum[i]; j++)
      if (profit[i][j] > max)
        max = profit[i][j];
    mnum[i] = 0;
    mval[i] = max;
    mplace[i][0] = 0;
    if (max == 0)
      mplace[i][mnum[i]++] = 0;
    for (j = 0; j < bnum[i]; j++)
      if (profit[i][j] == max)
        mplace[i][mnum[i]++] = j+1;
  }
}

void Bag(void)
{
  int minp = 0, maxp, i, j, k;
  int printed = 0, maxprofit = 0;
 
  memset(gotp, 0, sizeof(gotp));
  for (i = 0; i < WY; i++)
    minp += mplace[i][0];
  gotp[minp] = 1;
  maxp = minp;
  for (j = 0; j < WY; j++)
    for (i = minp; i <= maxp; i++) {
      if (gotp[i] == 2)
        gotp[i] = 1;
      else if (gotp[i] == 1)
        for (k = 1; k < mnum[j]; k++)
          if (!gotp[i - mplace[j][0] + mplace[j][k]]) {
            gotp[i - mplace[j][0] + mplace[j][k]] = 2;
            if (i - mplace[j][0] + mplace[j][k] > maxp)
	      maxp = i - mplace[j][0] + mplace[j][k];
          }
    }
  for (i = 0; i < WY; i++)
    maxprofit += mval[i];
  printf("Maximum profit is %d.\n", maxprofit);
  printf("Number of pruls to buy:");
  for (i = 0; i <= maxp && printed < 10; i++)
    if (gotp[i]) {
      printf(" %d", i);
      printed++;
    }
  printf("\n");
}

int main(void)
{
  int n = 1;
  
  while (1) {
    ReadInp();
    if (n != 1)
      printf("\n");
    GatherMax();
    printf("Workyards %d\n", n);
    Bag();
    n++;
  }
  return 0;
}

