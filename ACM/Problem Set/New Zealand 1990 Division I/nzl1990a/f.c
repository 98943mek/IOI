#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#define MAX 1024
#define WIDTH 128
#define eq(a,b) (fabs((a)-(b))<0.0001)

struct { int x,y; } poly1[MAX],poly2[MAX];
int p1,p2;
struct _mriz { float y1,y2;
	      int smer; } lines1[WIDTH][MAX],lines2[WIDTH][MAX];
int pocet1[WIDTH],pocet2[WIDTH];
float area1,area2,area0,a;
int i,j,k;

float podobema(float p1,float p2,float q1,float q2) {
 float r;
 if (((p1<q1)||eq(p1,q1))&&((p2<q2)||eq(p2,q2))) return (p1+p2)*0.5;
 if (((p1>q1)||eq(p1,q1))&&((p2>q2)||eq(p2,q2))) return (q1+q2)*0.5;
 r=(q2-p2)/(p1-p2-q1+q2);
 if (p1<q1)
  return r*(p1+p2)*0.5+(1-r)*(q1+q2)*0.5;
 else
  return r*(q1+q2)*0.5+(1-r)*(p1+p2)*0.5;
 }
float podjednim(float p1,float p2,float q1,float q2) {
 float r;
 if (((p1<q1)||eq(p1,q1))&&((p2<q2)||eq(p2,q2))) return (q1+q2)*0.5;
 if (((p1>q1)||eq(p1,q1))&&((p2>q2)||eq(p2,q2))) return (p1+p2)*0.5;
 r=(q2-p2)/(p1-p2-q1+q2);
 if (p1<q1)
  return r*(q1+q2)*0.5+(1-r)*(p1+p2)*0.5;
 else
  return r*(p1+p2)*0.5+(1-r)*(q1+q2)*0.5;
 }

int main() {
 scanf("%d",&p1);
 while (p1) {
  for (i=0; i<p1; i++) scanf("%d %d",&poly1[i].x,&poly1[i].y);
  scanf("%d",&p2);
  for (i=0; i<p2; i++) scanf("%d %d",&poly2[i].x,&poly2[i].y);
  for (i=0; i<WIDTH; i++) pocet1[i]=pocet2[i]=0;
  for (i=0; i<p1; i++) {
   j=(i+1)%p1;
   if (poly1[i].x<poly1[j].x)
    for (k=poly1[i].x-1;k<poly1[j].x-1;k++) {
     lines1[k][pocet1[k]].y1=1.0*(poly1[j].y-poly1[i].y)/(poly1[j].x-poly1[i].x)*(k+1-poly1[i].x)+poly1[i].y;
     lines1[k][pocet1[k]].y2=1.0*(poly1[j].y-poly1[i].y)/(poly1[j].x-poly1[i].x)*(k+2-poly1[i].x)+poly1[i].y;
     lines1[k][pocet1[k]++].smer=1;
     }
   else
    if (poly1[i].x>poly1[j].x)
     for (k=poly1[j].x-1;k<poly1[i].x-1;k++) {
      lines1[k][pocet1[k]].y1=1.0*(poly1[j].y-poly1[i].y)/(poly1[j].x-poly1[i].x)*(k+1-poly1[i].x)+poly1[i].y;
      lines1[k][pocet1[k]].y2=1.0*(poly1[j].y-poly1[i].y)/(poly1[j].x-poly1[i].x)*(k+2-poly1[i].x)+poly1[i].y;
      lines1[k][pocet1[k]++].smer=-1;
      }
   }
  for (i=0; i<p2; i++) {
   j=(i+1)%p2;
   if (poly2[i].x<poly2[j].x)
    for (k=poly2[i].x-1;k<poly2[j].x-1;k++) {
     lines2[k][pocet2[k]].y1=1.0*(poly2[j].y-poly2[i].y)/(poly2[j].x-poly2[i].x)*(k+1-poly2[i].x)+poly2[i].y;
     lines2[k][pocet2[k]].y2=1.0*(poly2[j].y-poly2[i].y)/(poly2[j].x-poly2[i].x)*(k+2-poly2[i].x)+poly2[i].y;
     lines2[k][pocet2[k]++].smer=1;
     }
   else
    if (poly2[i].x>poly2[j].x)
     for (k=poly2[j].x-1;k<poly2[i].x-1;k++) {
      lines2[k][pocet2[k]].y1=1.0*(poly2[j].y-poly2[i].y)/(poly2[j].x-poly2[i].x)*(k+1-poly2[i].x)+poly2[i].y;
      lines2[k][pocet2[k]].y2=1.0*(poly2[j].y-poly2[i].y)/(poly2[j].x-poly2[i].x)*(k+2-poly2[i].x)+poly2[i].y;
      lines2[k][pocet2[k]++].smer=-1;
      }
   }
  area1=area2=area0=0;
  for (i=0; i<WIDTH; i++) {
   for (j=0; j<pocet1[i]; j++) area1+=(lines1[i][j].y1+lines1[i][j].y2)*lines1[i][j].smer*0.5;
   for (j=0; j<pocet2[i]; j++) area2+=(lines2[i][j].y1+lines2[i][j].y2)*lines2[i][j].smer*0.5;
   for (j=0; j<pocet1[i]; j++)
    for (k=0; k<pocet2[i]; k++) {
     a=podobema(lines1[i][j].y1,lines1[i][j].y2,lines2[i][k].y1,lines2[i][k].y2);
     area0+=a*lines1[i][j].smer*lines2[i][k].smer;
     a=podjednim(lines1[i][j].y1,lines1[i][j].y2,lines2[i][k].y1,lines2[i][k].y2);
     area0-=a*lines1[i][j].smer*lines2[i][k].smer;
     }
   }
  if eq(area1+area2-area0,0)
   printf("    0.00\n");
  else
   printf("%8.2f\n",area1+area2-area0);
  scanf("%d",&p1);
  }
 return 0;
}
