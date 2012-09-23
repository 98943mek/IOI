// Practice Session ACM-ICPC Asia Phuket 2011 (3 Nov 2011)
// Reverse and Sort by Seksun

#include <iostream>
#include <stdlib.h>
using namespace std;

#define MAX 100000
typedef unsigned long long Int64; //define 64-bit interger to cover the value 10^12

int compare_fn(const void *a, const void *b)
{ Int64 *x =(Int64 *) a;
  Int64 *y =(Int64 *) b;
  if(*x == *y) return 0;
  else if(*x > *y) return 1;
  else return -1;
}

Int64 reverseInt(Int64 x)
{ Int64 res=0,digit;
  while(x>0)  
  { digit=x%10;
    res=10*res+digit;
    x=x/10;
  }
  return res;
}
int main()
{   Int64 x[MAX];
    int n,i,m; 
    cin >> n;
    i=0; m=n;
    while (n>0)
    { cin >> x[i]; 
      //cout << x[i] << endl; //debug line
      x[i]=reverseInt(x[i]);
      i++; n--;
    }
    qsort(x,m,sizeof(Int64),compare_fn); // Sorting (asc)
    n=m; i=0;  
    while (n>0)
    { cout << x[i] << endl; //output line
      i++; n--;
    }
    //cin >> m; //debug
    return 0;
}
