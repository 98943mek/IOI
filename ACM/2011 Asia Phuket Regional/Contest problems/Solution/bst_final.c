/*    ACM-ICPC Asia Phuket Regional 2011
  // Binary Search Tree : Final Solution */
#include <stdio.h>
#include <stdlib.h>

#define MAX 10000
typedef struct btnode
{ int data;
struct btnode *leftTree;
struct btnode *rightTree;
} TREENODE;

typedef TREENODE *TREE; 

int tsize(TREE t);
TREE insert(TREE t, int x);
TREE addNode(int value, TREE left, TREE right)
{ TREE rootNode;
rootNode = (TREENODE *) malloc(sizeof(TREENODE));
rootNode->data = value;
rootNode->leftTree = left;
rootNode->rightTree = right;
return rootNode;
}


int isEmpty(TREE t)
{ return (t==NULL); }

void printInorder(TREE t)
{ if (isEmpty(t)) ; /* //printf(""); */
  else
   { printInorder(t->leftTree);
     printf("%d\n", t->data);
     printInorder(t->rightTree);
    }
}
void printPreorder(TREE t)
{ if (isEmpty(t)) ; /*//printf(""); */
   else
    { printf("%d\n", t->data);
      printPreorder(t->leftTree);
      printPreorder(t->rightTree); 
    }
}
void printPostorder(TREE t)
{ if (isEmpty(t)) ;  /*//printf(""); */
  else
   {  printPostorder(t->leftTree);
      printPostorder(t->rightTree);
      printf("%d\n", t->data);
   }
}

/* //----------------main------------------- */
int main (){
	int n,i,num;
	TREE troot = NULL;

    /* //Constructing a BST */
    n=0;
    while ( scanf("%d",&num)>0 && n<MAX )
	{
	  if(num>=0) troot = insert(troot,num);
	  else break;
	  n++;
    }
	
 	printPostorder(troot); 
    
	return 0;
}

/* // insert BST node */
TREE insert(TREE t , int x){
	TREE troot;
	if(t == NULL){
		troot = (TREENODE *) malloc(sizeof(TREENODE));
		troot->data = x;
		troot->leftTree = NULL;
		troot->rightTree = NULL;
		t = troot;
	}
	else if(x < t->data)
		t->leftTree = insert(t->leftTree,x);
	else if(x > t->data)
		t->rightTree = insert(t->rightTree,x);
	
	return t;
}
int tsize(TREE t){
	
	if (isEmpty(t))
		return 0;
	else
	{ 
		return (1 + tsize(t->leftTree) + tsize(t->rightTree));
	}
}

