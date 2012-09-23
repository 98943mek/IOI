#include <stdio.h>

#define MAXN 50
#define SGN(A) (((A) <= 0) ? -1 : 1)

struct point {
  double C[3];
};

int N;
double D;
struct point S1, S2, V;
int sgnv[3];
double Bottom[MAXN][MAXN][MAXN];

void ReadInput(void)
{
  if (scanf("%d %lf %lf %lf %lf %lf %lf %lf", &N, &D, &S1.C[0], &S1.C[1], &S1.C[2], &S2.C[0], &S2.C[1], &S2.C[2]) != 8)
    exit(0);
}

double CountVolume(void)
{
  int i, j, k;
  double Vol = 0;

  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      for (k = 0; k < N; k++) {
        Vol += 1-Bottom[i][j][k];
/*        if (Bottom[i][j][k] != 1)
	  printf("%d, %d, %d = %f\n", i, j, k, Bottom[i][j][k]);*/
      }
  return Vol*D*D*D;
}

#define whole(A) (((A)+100.0-(int)((A)+100.0)) < 1e-6)

void Update(float fx, float fy, float fz, double d)
{
  int x = fx, y = fy, z = fz;
  if (fx < 0 || fy < 0 || fz < 0 || x < 0 || y < 0 || z < 0 || x >= N || y >= N || z >= N)
    return;
  d -= (int)d;
/*  printf("Hit: %d %d %d - %f\n", x, y, z, d);*/
  if (Bottom[x][y][z] > d)
    Bottom[x][y][z] = d;
}

void ScanCubes(int c)
{
  int i;
  double a[3]; 
  int ch[3] = {0, 0, 0};
  double k;

  ch[c] = -1;
  for (i = 0; i <= N; i++) {
    k = (((float)i) - S1.C[c])/V.C[c];
    if (k < 0)
      return;
    a[0] = V.C[0]*k + S1.C[0];
    a[1] = V.C[1]*k + S1.C[1];
    a[2] = V.C[2]*k + S1.C[2];
    if (whole(a[0]) && whole(a[1]) && whole(a[2])) {
      Update(a[0], a[1], a[2], a[2]);
      Update(a[0]+sgnv[0], a[1], a[2], a[2]);
      Update(a[0], a[1]+sgnv[1], a[2], a[2]);
      Update(a[0], a[1], a[2]+sgnv[2], a[2]);
      Update(a[0]+sgnv[0], a[1]+sgnv[1], a[2], a[2]);
      Update(a[0]+sgnv[0], a[1], a[2]+sgnv[2], a[2]);
      Update(a[0], a[1]+sgnv[1], a[2]+sgnv[2], a[2]);
      Update(a[0]+sgnv[0], a[1]+sgnv[1], a[2]+sgnv[2], a[2]);
    }
    else if (whole(a[0]) && c != 0) {
      Update(a[0], a[1], a[2], a[2]);
      Update(a[0]+ch[0], a[1]+ch[1], a[2]+ch[2], a[2]);
      Update(a[0]+sgnv[0], a[1], a[2], a[2]);
      Update(a[0]+sgnv[0]+ch[0], a[1]+ch[1], a[2]+ch[2], a[2]);
    }
    else if (whole(a[1]) && c != 1) {
      Update(a[0], a[1], a[2], a[2]);
      Update(a[0]+ch[0], a[1]+ch[1], a[2]+ch[2], a[2]);
      Update(a[0], a[1]+sgnv[1], a[2], a[2]);
      Update(a[0]+ch[0], a[1]+sgnv[1]+ch[1], a[2]+ch[2], a[2]);
    }
    else if (whole(a[0]) && c != 2) {
      Update(a[0], a[1], a[2], a[2]);
      Update(a[0]+ch[0], a[1]+ch[1], a[2]+ch[2], a[2]);
      Update(a[0], a[1], a[2]+sgnv[2], a[2]);
      Update(a[0]+ch[0], a[1]+ch[1], a[2]+ch[2]+sgnv[2], a[2]);
    }
    else {
      Update(a[0], a[1], a[2], a[2]);
      Update(a[0]+ch[0], a[1]+ch[1], a[2]+ch[2], a[2]);
    }
  }
}

int main(void)
{
  int i, j, k, Case = 1;

  while (1) {
    ReadInput();
    if (Case != 1)
      printf("\n");
    D /= (double)N;
    S1.C[0] /= D; S1.C[1] /= D; S1.C[2] /= D;
    S2.C[0] /= D; S2.C[1] /= D; S2.C[2] /= D;
    V.C[0] = S2.C[0] - S1.C[0]; V.C[1] = S2.C[1] - S1.C[1]; V.C[2] = S2.C[2] - S1.C[2];
    sgnv[0] = SGN(V.C[0]);
    sgnv[1] = SGN(V.C[1]);
    sgnv[2] = SGN(V.C[2]);
/*    printf("V=(%f, %f, %f), S=(%f, %f, %f)\n", V.C[0], V.C[1], V.C[2], S1.C[0], S1.C[1], S1.C[2]);*/
    for (i = 0; i < N; i++)
      for (j = 0; j < N; j++)
        for (k = 0; k < N; k++)
	  Bottom[i][j][k] = 1;
    ScanCubes(0);
    ScanCubes(1);
    ScanCubes(2);
    printf("Trial %d, Volume = %.2f\n", Case, CountVolume());
    Case++;
  }
  return 0;
}

