
#include<stdio.h>
#include<math.h>

#define min(a,b) (((a)>(b))?(b):(a))
#define abs(a)   (((a)>(-(a)))?(a):(-(a)))


void convert( int bod, int *x, int *y, int *z )
{
int a;
int b;
int c, str, zb;
if( bod==1 )
  {
  *x=0;
  *y=0;
  *z=0;
  return;
  }
a=(bod-2)/6;
b=(-1.0+sqrt(1+8*a))/2.0;
c=bod-1-3*b*(b+1);
++b;
str=(c-1)/b;
zb=c-str*b; 
switch( str )
  {
  case 0: *x=-zb; *y=-b; break;
  case 1: *x=-b; *y=zb-b; break;
  case 2: *x=zb-b; *y=zb; break;
  case 3: *x=zb; *y=b; break;
  case 4: *x=b; *y=b-zb; break;
  case 5: *x=b-zb; *y=-zb; break;
  }
*z=*x-*y;
return;  
}

int main(void)
{
int a, b, ax, ay, az, bx, by, bz, x, y ,z;
int pom;
scanf( "%d %d\n", &a, &b );
while( a!=0 || b!=0 )
  {
  convert( a, &ax, &ay, &az );
  convert( b, &bx, &by, &bz );
/*  printf( "%d %d %d\n", ax, ay, az );
  printf( "%d %d %d\n", bx, by, bz );*/
  x=abs(ax-bx);
  y=abs(ay-by);
  z=abs(az-bz);
  pom=min(x+y,x+z);
  printf( "The distance between cells %d and %d is %d.\n", a, b, min(pom,y+z) );
  scanf( "%d %d\n", &a, &b );
  }
return 0;
}
