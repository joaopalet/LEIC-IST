/*
 ASA 2017/2018 - Projeto 1
 Joao Palet Almeida 86447
 Maria Forjo 86475
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node *link;

typedef struct node{
  int       id;
  link      next;
} Node;

typedef struct aresta{
  int       from;
  int       to;
} *Aresta;

typedef struct stck{
  int      *items;
  int       top;
} Stack;

int         N, M;
link        *adjList;
Aresta      arestas;
Stack       stack;
int         *d;
int         *low;
int         *on_stack;
int         *SCC;
Aresta      connections;
int         visited = 1;
int         n_components = 0;

link        insertEdge(int v, link next);
void        Tarjan();
void        TarjanVisit(int u);
void        push(int i);
int         pop();
int         min(int a, int b);
int         comparator (const void * a, const void * b);



/*--------------------------------------------------------------------
| Function: main
---------------------------------------------------------------------*/

int main () {
	scanf("%d\n%d", &N, &M);

  /* Inicializacao da lista de adjacencias */
  adjList = (link*) malloc((N+1) * sizeof(link));

  stack.items = (int*) malloc(sizeof(int) * N);
  stack.top = -1;

  d = (int*) malloc(sizeof(int) * (N+1));

  low = (int*) malloc(sizeof(int) * (N+1));

  on_stack = (int*) malloc(sizeof(int) * (N+1));

  SCC = (int*) malloc(sizeof(int) * (N+1));

  arestas = (Aresta) malloc(sizeof(struct aresta) * M);

  int i;

  for (i = 0; i < N; i++) {
    adjList[i] = NULL;
    stack.items[i] = -1;
  }

  for (i = 0; i < M; i++) {
    int u, v;
    scanf("%d %d\n", &u, &v);

    adjList[u] = insertEdge(v, adjList[u]);

    arestas[i].from = u;
    arestas[i].to = v;
  }

  Tarjan();

  /* Associa a cada vertice o representante da respetiva componente */
  int representantes[n_components + 1];

  for (i = 0; i < n_components; i++)
    representantes[i] = -1;

  for (i = 1; i < N+1; i++) {
    if (representantes[SCC[i]] == -1) {
      representantes[SCC[i]] = i;
      SCC[i] = i;
    }
    else {
      SCC[i] = representantes[SCC[i]];
    }
  }



  struct aresta connections[M];
  int all_connections = 0;

  for (i = 0; i < M; i++) {
    if (SCC[arestas[i].from] != SCC[arestas[i].to]) {
      connections[all_connections].from = SCC[arestas[i].from];
      connections[all_connections].to = SCC[arestas[i].to];
      all_connections++;
    }
  }

  qsort(connections, all_connections, sizeof(struct aresta), comparator);

  int n_connections = all_connections;

  for (i = 1; i < all_connections; i++) {
    if (connections[i].from == connections[i-1].from && connections[i].to == connections[i-1].to) {
      connections[i-1].from = -1;
      n_connections--;
    }
  }

  printf("%d\n%d\n", n_components, n_connections);

  for (i = 0; i < all_connections; i++)
    if (connections[i].from != -1)
      printf("%d %d\n", connections[i].from, connections[i].to);


  free(adjList);
  free(stack.items);
  free(d);
  free(low);
  free(on_stack);
  free(SCC);
  free(arestas);

  return 0;
}



/*--------------------------------------------------------------------
| Function: tarjan
---------------------------------------------------------------------*/

void Tarjan() {
  int i;

  for (i = 1; i <= N; i++) {
    d[i] = -1;
    low[i] = -1;
    on_stack[i] = 0;
  }

  for (i = 1; i <= N; i++) {
    if (d[i] == -1)
      TarjanVisit(i);
  }
}



/*--------------------------------------------------------------------
| Function: tarjanVisit
---------------------------------------------------------------------*/

void TarjanVisit(int u) {
  d[u] = visited;
  low[u] = visited;
  visited++;

  push(u);
  on_stack[u] = 1;

  link v;

  for (v = adjList[u]; v != NULL; v = v->next) {

    if (d[v->id] == -1 || on_stack[v->id]) {
      if (d[v->id] == -1) {
        TarjanVisit(v->id);
      }

      low[u] = min(low[u], low[v->id]);
    }
  }

  if (d[u] == low[u]) {
    int y;
    while ((y = pop()) != u) {

      on_stack[y] = 0;
      SCC[y] = n_components;
    }
    SCC[y] = n_components;
    on_stack[y] = 0;

    n_components ++;
  }
}



/*--------------------------------------------------------------------
| Function: pop
---------------------------------------------------------------------*/

int pop() {
  if (stack.top == -1) {
    printf("\nStack underflow\n");
    exit(1);
  }

  return (stack.items[stack.top--]);
}



/*--------------------------------------------------------------------
| Function: push
---------------------------------------------------------------------*/

void push(int v) {
  if (stack.top == N-1) {
    printf("\nStack overflow\n");
    exit(1);
  }

  stack.items[++(stack.top)] = v;
}



/*--------------------------------------------------------------------
| Function: insertEdge
---------------------------------------------------------------------*/

link insertEdge(int v, link next) {
  link x = (link) malloc(sizeof(Node));

  x->id = v;
  x->next = next;

  return x;
}



/*--------------------------------------------------------------------
| Function: min
---------------------------------------------------------------------*/

int min(int a, int b) {
    return a < b ? a : b;
}



/*--------------------------------------------------------------------
| Function: comparator
---------------------------------------------------------------------*/

int comparator (const void * a, const void * b) {
  struct aresta x = *(Aresta)a;
  struct aresta y = *(Aresta)b;

  if (x.from == y.from)
    return x.to - y.to;

  else
    return x.from - y.from;
}
