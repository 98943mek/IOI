#include <cstdio>
#include <vector>

using namespace std;

#define MAX_N     1000

struct Edge {
  int f,t;
  int c,oc;
  int rev_index;

  Edge(int ff, int tt, int cc, int ri)
    :f(ff), t(tt), oc(cc), c(cc), rev_index(ri) {}
};

vector<int> adj[MAX_N];
vector<int> inc_edges[MAX_N];
vector<Edge> edges;
int n, m;

void read_input()
{
  scanf("%d %d",&n,&m);
  for(int i=0; i<n; i++) {
    adj[i].clear();
    inc_edges[i].clear();
    edges.clear();
  }

  for(int i=0; i<m; i++) {
    int u,v,b;
    scanf("%d %d %d",&u,&v,&b);
    u--; v--;
    adj[u].push_back(v);
    adj[v].push_back(u);

    edges.push_back(Edge(u,v,b,i*2+1));
    edges.push_back(Edge(v,u,0,i*2));

    inc_edges[u].push_back(i*2);
    inc_edges[v].push_back(i*2+1);
  }
}

int path[MAX_N];
int plen;
bool visited[MAX_N];

bool find_path(int u, int t)
{
  if(u==t)
    return true;

  visited[u] = true;
  for(int i=0; i<adj[u].size(); i++) {
    int v = adj[u][i];
    int ei = inc_edges[u][i];
    if((!visited[v]) && (edges[ei].c>0)) {
      path[plen] = ei;
      plen++;
      if(find_path(v,t))
	return true;
      plen--;
    }
  }
  return false;
}

bool do_find(int s, int t)
{
  for(int i=0; i<n; i++)
    visited[i] = false;
  plen = 0;
  return find_path(s,t);
}

int augment()
{
  int min_cap = edges[path[0]].c;
  for(int i=0; i<plen; i++)
    if(edges[path[i]].c < min_cap)
      min_cap = edges[path[i]].c;
  for(int i=0; i<plen; i++) {
    int e = path[i];
    int er = edges[e].rev_index;
    edges[e].c -= min_cap;
    edges[er].c += min_cap;
  }
  return min_cap;
}

void init_max_flow()
{
  for(int i=0; i<m*2; i++)
    edges[i].c = edges[i].oc;
}

int find_max_flow()
{
  int mf = 0;

  while(do_find(0,n-1)) {
    mf += augment();
  }
  return mf;
}

main()
{ double cl=clock();
  int k;
  scanf("%d",&k);
  for(int kk=0; kk<k; kk++) {
    read_input();
    int mf = find_max_flow();
    //printf("mf = %d\n",mf);
    int ecount = 0;
    for(int i=0; i<m; i++) {
      init_max_flow();
      edges[i*2].c--;
      int ff = find_max_flow();
      //printf("%d %d\n",i,ff);
      if(ff<mf)
	ecount++;
    }
    printf("%d\n",ecount);
  }
  cl = clock() - cl;
  fprintf(stderr, "\n%lf\n", cl / 1000);
}




