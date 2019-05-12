/*
 ASA 2017/2018 - Projeto 2
 Joao Palet 86447
 Maria Forjo 86475
*/


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node *link;

typedef struct node {
  int       v;
  link      next;
} Node;

typedef struct q {
  int      *items;
  int       last;
  int       first;
  int       numElements;
} Queue;

int         M;  /* linhas */
int         N;  /* colunas */
int         source;
int         target;
int*        lp; /* pesos de plano */
int*        cp; /* pesos de cenario */
int**       capacity;
int**       flow;
int*        parents;
int*        pathCapacity;
link*       adjList;
Queue       queue;
int*        colors;
int         end = 0;

int** mallocMatrix(int linhas,int colunas);
link insertEdge(int v, link next);
int edmondsKarp(int source, int sink);
int bfs(int noInicial, int noFinal);
int min(int a, int b);
void queuePush(int v);
int queuePop();



int main() {
  int i, j, k, c;

  scanf("%d %d", &M, &N);
  source = 0;
  target = N*M + 1;

  lp = (int*) malloc(sizeof(int) * N * M);
  cp = (int*) malloc(sizeof(int) * N * M);
  capacity = mallocMatrix(N*M+2, N*M+2);
  flow = mallocMatrix(N*M+2, N*M+2);
  parents = (int*) malloc(sizeof(int) * (N * M) + 2);
  pathCapacity = (int*) malloc(sizeof(int) * (N * M) + 2);
  adjList = (link*) malloc(sizeof(link) * (N * M) + 2);
  colors = (int*) malloc(sizeof(int) * (N * M) + 2);

  queue.items = (int*) malloc(sizeof(int) * N * M + 2);

  for(i = 1; i < target; i++) {
    scanf("%d", &lp[i]);
    capacity[0][i] = lp[i];
    adjList[0] = insertEdge(i, adjList[0]);
    colors[i] = 0;
  }

  for(i = 1; i < target; i++) {
    scanf("%d", &cp[i]);
    capacity[i][target] = cp[i];
    adjList[i] = insertEdge(target, adjList[i]);
  }

  k = 1;
  for(i = 0; i < M; i++) {
    for(j = 0; j < N - 1; j++) {
      scanf("%d", &c);
      capacity[k][k+1] = c;
      adjList[k] = insertEdge(k+1, adjList[k]);
      capacity[k+1][k] = c;
      adjList[k+1] = insertEdge(k, adjList[k+1]);
      k++;
    }
    k++;
  }

  k = 1;
  for(i = 0; i < M - 1; i++) {
    for(j = 0; j < N; j++) {
      scanf("%d", &c);
      capacity[k][k+N] = c;
      adjList[k] = insertEdge(k + N, adjList[k]);
      capacity[k+N][k] = c;
      adjList[k+N] = insertEdge(k, adjList[k+N]);
      k++;
    }
  }

  int maxflow = edmondsKarp(source, target);
  printf("%d\n\n", maxflow);

  k = 1;
  for(i = 1; i <= M; i++) {
    for(j = 1; j <= N; j++) {
      if(colors[k] == 0)
        printf("P ");
      else
        printf("C ");
      k++;
    }
    printf("\n");
  }

  return 0;
}



/*----------------------------------------------------------------
Function : mallocMatrix
-----------------------------------------------------------------*/

int** mallocMatrix(int linhas, int colunas) {
  int i,j;

  int **m = (int**) malloc(linhas * sizeof(int*));

  for (i = 0; i < linhas; i++) {
       m[i] = (int*) malloc(colunas * sizeof(int));

       for (j = 0; j < colunas; j++){
            m[i][j] = 0;
       }
  }

  return m;
}



/*--------------------------------------------------------------------
| Function: insertEdge
---------------------------------------------------------------------*/
link insertEdge(int v, link next) {
  link x = (link) malloc(sizeof(Node));

  x->v = v;
  x->next = next;

  return x;
}



/*----------------------------------------------------------------
Function : queuePush
-----------------------------------------------------------------*/
void queuePush(int v) {
  queue.numElements++;
  queue.items[queue.last++] = v;
}



/*----------------------------------------------------------------
Function : queuePop
-----------------------------------------------------------------*/
int queuePop() {
  queue.numElements--;
  return queue.items[queue.first++];
}



/*--------------------------------------------------------------------
| Function: min
---------------------------------------------------------------------*/

int min(int a, int b) {
    return a < b ? a : b;
}



/*----------------------------------------------------------------
Function : bfs
-----------------------------------------------------------------*/
int bfs(int noInicial, int noFinal){
  int i;
  link j;

  for(i = 0; i < N * M + 2; i++) {
    parents[i] = -1;
    pathCapacity[i] = 0;
  }

  queuePush(noInicial);

  parents[noInicial] = -2;
  pathCapacity[noInicial] = 1000;

  while (queue.numElements) {
    int current = queuePop();

    for (j = adjList[current]; j != NULL; j = j->next) {
      int to = j->v;

      if(parents[to] == -1) {
        if(capacity[current][to] - flow[current][to] > 0) {
          parents[to] = current;

          if(end == 1)
            colors[to] = 1;

          pathCapacity[to] = min(pathCapacity[current], capacity[current][to] - flow[current][to]);

          if(to == noFinal)
            return pathCapacity[noFinal];

          queuePush(to);
        }
      }
    }
  }

  return 0;
}



/*----------------------------------------------------------------
Function : edmondsKarp
-----------------------------------------------------------------*/
int edmondsKarp(int source, int sink){

  int maxFlow = 0;

  while (1){

    queue.first = 0;
    queue.last = 0;
    queue.numElements = 0;

  	int flowAtual = bfs(source, sink);

  	if (flowAtual == 0) break;

  	maxFlow += flowAtual;

  	int noAtual = sink;

  	while (noAtual != source){
  		int noAnterior = parents[noAtual];

  		flow[noAnterior][noAtual] += flowAtual;

  		flow[noAtual][noAnterior] += flowAtual;

  		noAtual = noAnterior;
  	}

  }

  end = 1;

  bfs(source, sink);

  return maxFlow;
}
