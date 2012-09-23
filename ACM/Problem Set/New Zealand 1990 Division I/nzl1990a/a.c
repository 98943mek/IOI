#include<stdio.h>
#define TOP 100

typedef long lotspolongs[TOP+1];

lotspolongs px,py,hx,hy,hull;
long n,m,start;
long x,y,minx,miny,maxx,maxy;
long x0,y0,xc,yc,width,height;
char thing[50];

long morepolongs() {
 return (x>0)&&(y>0);
 }

long ccw(long s,long m,long e) {
 long dx1,dx2,dy1,dy2,cc;
 dx1=px[m]-px[s]; dy1=py[m]-py[s];
 dx2=px[e]-px[s]; dy2=py[e]-py[s];
 if (dx1*dy2>dy1*dx2) cc=-1;
 if (dx1*dy2<dy1*dx2) cc=1;
 if (dx1*dy2==dy1*dx2) cc=0;
 return cc;
 }

void grahamscan(){
 long i;
 start=1;
 for (i=2; i<=n; i++)
  if (py[i]<py[start]) start=i;
 for (i=1; i<=n; i++)
  if (py[i]==py[start]) start=i;
 hull[1]=start;
 for (i=2; i<=n; i++) {
  start=(start==n) ? 1 : start+1;
  hull[i]=start;
  }
 hull[0]=n; hull[++n]=hull[1];
 m=3; start=4;
 while (!ccw(hull[m-2],hull[m-1],hull[m])) {
  hull[m-1]=hull[m];
  hull[m]=hull[start++];
  }
 for (i=start; i<=n; i++) {
  while (ccw(hull[m-1],hull[m],hull[i])>=0) m--;
  hull[++m]=hull[i];
  }
 }

void findpolongs() {
 long x1,y1,x2,y2,a2,b2,c2;
 long i,k;
 scanf("%ld %ld",&xc,&yc);
 n=0; scanf("%ld %ld",&x,&y);
 minx=x; miny=y;
 maxx=x; maxy=y;
 while (morepolongs()) {
  px[++n]=x; py[n]=y;
  if (minx>x) minx=x;
  if (miny>y) miny=y;
  if (maxx<x) maxx=x;
  if (maxy<y) maxy=y;
  scanf("%ld %ld",&x,&y);
  }
 width=maxx-minx; height=maxy-miny;
 grahamscan();
 start=1;
 for (i=2; i<=m; i++)
  if (hull[i]<hull[start]) start=i;
 i=start; k=hull[i];
 x2=px[k]; y2=py[k];
 c2=(x2-xc)*(x2-xc)+(y2-yc)*(y2-yc);
 do {
  i=(i<m) ? i+1 : 1;
  k=hull[i];
  x1=x2; y1=y2;
  x2=px[k]; y2=py[k];
  a2=(x1-x2)*(x1-x2)+(y1-y2)*(y1-y2);
  b2=c2;
  c2=(x2-xc)*(x2-xc)+(y2-yc)*(y2-yc);
  }
 while (a2*a2-b2*b2-c2*c2+2*b2*c2<=0);
 printf("%-20s%2ld\n",thing,k);
 }

int main() {
 gets(thing);
 while (thing[0]!='#') {
  findpolongs();
  scanf("\n");
  gets(thing);
  }
 return 0;
}
