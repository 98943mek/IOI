#include<stdio.h>
#include<stdlib.h>

typedef struct _workseeker workseeker;
typedef workseeker *next;
struct _workseeker { int id;
		     next left,right;
		     };
next clock,anti,p1,q1,p2,q2;
int i,N,m,k,count,dead;

int main() {
 scanf("%d %d %d",&N,&k,&m);
 while (N) {
 anti=(next)malloc(sizeof(workseeker));
 p1=anti; anti->id=1;
 for (i=2; i<=N; i++) {
  q1=(next)malloc(sizeof(workseeker));
  q1->id=i;
  q1->left=p1;
  p1->right=q1;
  p1=q1;
  }
 clock=p1; p1->right=anti; anti->left=p1;
 for (count=N; count; ) {
  for (i=0; i<((k-1)%count); i++) anti=anti->right;
  for (i=0; i<((m-1)%count); i++) clock=clock->left;
  if (clock==anti) {
   printf("%3d",anti->id); dead=1;
   anti->right->left=anti->left;
   anti->left->right=anti->right;
   p1=anti;
   anti=anti->right;
   clock=clock->left;
   free(p1);
   }
  else {
   printf("%3d%3d",anti->id,clock->id); dead=2;
   p1=anti->right; if (p1==clock) p1=p1->right;
   q1=anti->left; if (q1==clock) q1=q1->left;
   p1->left=q1; q1->right=p1;
   p2=clock->right; if (p2==anti) p2=p2->right;
   q2=clock->left; if (q2==anti) q2=q2->left;
   p2->left=q2; q2->right=p2;
   q1=anti; p2=clock;
   anti=p1; clock=q2;
   free(q1); free(p2);
   }
  count-=dead;
  if (count) printf(",");
  }
 printf("\n");
 scanf("%d %d %d",&N,&k,&m);
 }
 return 0;
}


