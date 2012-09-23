#include <stdio.h>
#define MAX 20000

long ugly[MAX];
int dva,tri,pet;
int poradi;
int posl;

int main() {
 ugly[0]=1;
 dva=tri=pet=0;
 posl=1;
 scanf("%d",&poradi);
 while (poradi) {
  while (poradi>posl)
   if ((ugly[dva]*2<ugly[tri]*3)&&(ugly[dva]*2<ugly[pet]*5))
    if (ugly[dva]*2>ugly[posl-1])
     ugly[posl++]=ugly[dva++]*2;
    else
     dva++;
   else
    if (ugly[tri]*3<ugly[pet]*5)
     if (ugly[tri]*3>ugly[posl-1])
      ugly[posl++]=ugly[tri++]*3;
     else
      tri++;
    else
     if (ugly[pet]*5>ugly[posl-1])
      ugly[posl++]=ugly[pet++]*5;
     else
      pet++;
  printf("The %d'th ugly number is %ld.\n",poradi,ugly[poradi-1]);
  scanf("%d",&poradi);
 }
 return 0;
}
