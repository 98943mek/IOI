// Spelling Suggestion
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <string>
#include <set>
#include <map>

using namespace std;

int minedit(const string &s1,const string &s2);

// cost.h
#ifndef COST_H
#define COST_H

const int delcost=2;
const int inscost=2;
const int transcost=2;
int minof3(int a,int b,int c);
void init_subst_tab();
void set_close_subst(unsigned char c1,unsigned char c2);
int subscost(unsigned char c1,unsigned char c2);

#endif

// strie.h
#ifndef STRIE_H
#define STRIE_H

#include <string>
#include <set>
#include <map>
#include <vector>



class strie;

class node
{
friend class strie;
public:
   node(char c,node *parent,int depth);
   
   inline bool is_terminal()
   {
      return (w.size()!=0);
   }

private:
   char c;
   std::string w;
   node *parent;
   int depth;
   std::set<int> ssize;
   std::map<char,node> children;
   std::vector<int> d;
};

class nodeinheap
{
friend class strie;
public:
   nodeinheap(int h, node* p);
   inline bool operator<(const nodeinheap &o) const
   { return(h < o.h); }
   inline bool operator==(const nodeinheap &o) const
   { return(h == o.h); }
   inline bool operator!=(const nodeinheap &o) const
   { return(h != o.h); }
private:
   int h;
   node *p;
};

class strie
{
public:
   typedef std::map<char,node>::iterator child_iterator;
   typedef std::map<char,node>::const_iterator const_child_iterator;

   strie();
   bool insert(const std::string &word);
   int me_search(const std::string &word); //A* search
   bool exact_search(const std::string &word) const;
   const std::vector<std::string> &getresult() const;
   
private:
   void me_visit(node *c);
   int me_est(node *c); // Heuristic estimation

   node root;
   int maxl;
   int bestd;
   int worstd;
   int wsize;
   std::string w;
   std::multiset<nodeinheap> heap;
   std::vector<std::string> result;
};

#endif

// cost.cpp
const size_t TDIM=256;

static int subst_tab[TDIM][TDIM];

int minof3(int a,int b,int c)
{
   int m=a;
   if(b<m) m=b;
   if(c<m) m=c;
   return(m);
}

void init_subst_tab()
{
   for(size_t i=0;i<TDIM;i++)
      for(size_t j=0;j<TDIM;j++)
         if(i==j)
            subst_tab[i][j]=0;
         else
            subst_tab[i][j]=2;
   return;
}

void set_close_subst(unsigned char c1,unsigned char c2)
{
   subst_tab[c1][c2]=1;
   subst_tab[c2][c1]=1;
   return;
}

int subscost(unsigned char c1,unsigned char c2)
{
   return(subst_tab[c1][c2]);
}

// strie.cpp
node::node(char c,node *parent,int depth)
   : c(c),parent(parent),depth(depth)
{
}

nodeinheap::nodeinheap(int h, node* p) : h(h),p(p)
{
}


strie::strie() : root('\0',0,0),maxl(0)
{
}

bool strie::insert(const std::string &w)
{
   node *p=&root;
   bool is_new=false;
   // unnecessary for root
   // p->ssize.insert(w.size());
   if(w.size()>(size_t)maxl)
      maxl=w.size();
   for(size_t i=0;i<w.size();i++)
   {
      char c=w[i];
      pair<child_iterator,bool> t=p->children.insert(
            make_pair(c,node(c,p,i+1)));
      child_iterator it=t.first;
      p=&(it->second);
      int sleft=w.size()-i-1;
      if(sleft>0)
         p->ssize.insert(sleft);
   }
   if(p->w.size()==0)
   {
      is_new=true;
      p->w=w;
   }
   return(is_new);
}

int strie::me_search(const std::string &word)
{
   w=word;
   wsize=w.size();
   worstd=bestd=(wsize+maxl)*2;
   root.d=vector<int>(wsize+1);
   for(int i=0;i<=wsize;i++)
      root.d[i]=i*delcost;
   heap.clear();
   result.clear();

   me_visit(&root);

   while(!heap.empty())
   {
      multiset<nodeinheap>::iterator it=heap.begin();
      node *cur=it->p;
      if(it->h > bestd)
         break;
      heap.erase(it);
      me_visit(cur);
   }
   sort(result.begin(),result.end());
   return(bestd);
}

void strie::me_visit(node *c)
{
   for(child_iterator it=c->children.begin();it!=c->children.end();it++)
   {
      int h=me_est(&(it->second));
      node &child=(it->second);
      if(child.w.size()>0) // terminal node
      {
         int &curd=child.d[wsize];
         if(curd<bestd)
         {
            bestd=curd;
            result.clear();
            result.push_back(child.w);
/*  
//          Removing too high-cost nodes from heap
            multiset<nodeinheap>::iterator hit;
            hit=upper_bound(heap.begin(),heap.end(),nodeinheap(bestd,0));
            if(hit!=heap.end())
            {
               heap.erase(hit,heap.end());
            }
*/
         }
         else if(curd==bestd)
         {
            result.push_back(child.w);
         }
      }
      if(h<=bestd && child.children.size()>0)
      {
         heap.insert(nodeinheap(h,&(it->second)));
      }
   }
}

int strie::me_est(node *c)
{
   c->d.resize(wsize+1);
   vector<int> &d=c->d;
   d[0]=c->depth*inscost;
   node *p=c->parent;
   node *gp=p->parent;
   
   set<int>::const_iterator cs_it,ns_it;
   cs_it=ns_it=c->ssize.begin();
   if(ns_it!=c->ssize.end())
      ns_it++;

   int besth=worstd; // start with very high heuristic
   for(int i=1;i<=wsize;i++)
   {
      d[i]=minof3(p->d[i-1]+subscost(w[i-1],c->c),
                  d[i-1]+delcost,
                  p->d[i]+inscost);
      if(i>1 && c->depth>1 && w[i-1]!=c->c
      && w[i-1]==p->c && w[i-2]==c->c)
         d[i]=min(d[i],gp->d[i-2]+transcost);
   }
   for(int i=wsize-1;i>=1;i--)
   {
      int h=d[i];
      if(cs_it!=c->ssize.end()) // estimate heuristic
      {
         int sleft=wsize-i;
         int diffsize;
         if(sleft>*cs_it && ns_it!=c->ssize.end()) // find best size matching
         {
            if(sleft-*cs_it > *ns_it-sleft)
            {
               cs_it=ns_it;
               ns_it++;
            }
         }
         diffsize=(sleft<=*cs_it)?(*cs_it-sleft):(sleft-*cs_it);
         h=d[i]+diffsize*2;
      }
      if(h<besth)
         besth=h;
   }
   
   return(besth);
}

const std::vector<std::string> &strie::getresult() const
{
   return(result);
}

bool strie::exact_search(const std::string &word) const
{
   const node *cur=&root;
   size_t i;
   for(i=0;i<word.size();i++)
   {
      char c=word[i];
      const_child_iterator it=cur->children.find(c);
      if(it==cur->children.end())
         break;
      cur=&(it->second);
   }
   if(i==word.size() && cur->w.size()!=0)
      return(true);
   return(false);
}




int main(int argc, char *argv[])
{
   strie tree;
   string l;

   init_subst_tab();

   while(getline(cin,l)) // set close-substitution costs
   {
      if(l.size()==0)
         break;
      char c1=l[0];
      for(size_t i=2;i<l.size();i++)
         set_close_subst(c1,l[i]);
   }

   while(getline(cin,l)) // create dictionary data structures
   {
      if(l.size()==0)
         break;
      tree.insert(l);
   }

   while(getline(cin,l)) // search
   {
      if(l.size()==0)
         break;
      int minop=tree.me_search(l);
      const vector<string> &vs=tree.getresult();
      cout << l << ":" << minop << ":" << vs[0];
      for(size_t i=1;i<vs.size();i++)
         cout << ' ' << vs[i];
      cout << endl;
   }

   return(0);
}


