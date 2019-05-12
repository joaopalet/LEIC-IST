/*
Projeto SO - entrega 3

Francisco Sena - 86420
Joao Almeida - 86447
*/



#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

#include "matrix2d.h"


typedef struct {
    int id;
    int N;
    int iter;
    int trab;
    int tam_fatia;
    int maxD;
    int inicio;
    int fim;
    DoubleMatrix2D *matrixes[2];
}thread_info;


/*--------------------------------------------------------------------
| Variaveis globais
---------------------------------------------------------------------*/
int pendurados = 0, pendurados2 = 0, livre = 0, aborta = 1, iteracoesEfetuadas;

pthread_mutex_t mutex;
pthread_mutex_t mutex2;
pthread_mutex_t mutex3;


pthread_cond_t cond;
pthread_cond_t cond2;


/*--------------------------------------------------------------------
| Function: tarefa_trabalhadora
| Description: Função executada por cada tarefa trabalhadora.
|              Recebe como argumento uma estrutura do tipo thread_info.
---------------------------------------------------------------------*/


void *tarefa_trabalhadora(void* args) {

  thread_info *tinfo = (thread_info *) args;
  DoubleMatrix2D *matrixes[2];
  int atual, prox;
  int iter, i, j;

  if(tinfo->tam_fatia < 1 || tinfo->N < 2)
    return NULL;

  matrixes[0] = tinfo->matrixes[0];
  matrixes[1] = tinfo->matrixes[1];


  for (iter = 0; iter < tinfo->iter; iter++) {

    if(pthread_mutex_lock(&mutex) != 0) {
      fprintf(stderr, "\nErro ao bloquear mutex\n");
      exit(EXIT_FAILURE);
    }

    pendurados++;

    // quando a ultima tarefa chegar, acorda as outras e repoe as variaveis de controlo
    if (pendurados == tinfo->trab) {
      livre = 1;
      pendurados = 0;
      pendurados2 = 0;
      aborta = 1;
      iteracoesEfetuadas = iter;
      if(pthread_cond_broadcast(&cond) != 0) {
        fprintf(stderr, "\nErro ao desbloquear variável de condição\n");
        exit(EXIT_FAILURE);
      }
    }

    while(!livre)
    if(pthread_cond_wait(&cond, &mutex) != 0) {
      fprintf(stderr, "\nErro ao esperar pela variável de condição\n");
      exit(EXIT_FAILURE);
    }

    if(pthread_mutex_unlock(&mutex) != 0) {
      fprintf(stderr, "\nErro ao desbloquear mutex\n");
      exit(EXIT_FAILURE);
    }

    /* Calcular Pontos Internos */
    for (i = tinfo->inicio; i < tinfo->fim; i++) {
      for (j = 1; j < tinfo->N + 1; j++) {

        atual = iter % 2;
        prox = 1 - iter % 2;

        double value = (dm2dGetEntry(matrixes[atual], i, j+1) +
                      dm2dGetEntry(matrixes[atual], i, j-1) +
                      dm2dGetEntry(matrixes[atual], i+1, j) +
                      dm2dGetEntry(matrixes[atual], i-1, j))/4;
        dm2dSetEntry(matrixes[prox], i, j, value);

        if(aborta)
          if(value - dm2dGetEntry(matrixes[atual], i, j) >= tinfo->maxD) {
            if(pthread_mutex_lock(&mutex3) != 0) {
              fprintf(stderr, "\nErro ao bloquear mutex\n");
              exit(EXIT_FAILURE);
            }
            aborta = 0;
            if(pthread_mutex_unlock(&mutex3) != 0) {
              fprintf(stderr, "\nErro ao desbloquear mutex\n");
              exit(EXIT_FAILURE);
            }
          }
      }
    }

    if(pthread_mutex_lock(&mutex2) != 0) {
      fprintf(stderr, "\nErro ao bloquear mutex\n");
      exit(EXIT_FAILURE);
    }

    pendurados2++;

    // quando a ultima tarefa chegar, acorda as outras e bloqueia-se no inicio da nova iteracao
    if (pendurados2 == tinfo->trab) {
      livre = 0;
      if(pthread_cond_broadcast(&cond2) != 0) {
        fprintf(stderr, "\nErro ao desbloquear variável de condição\n");
        exit(EXIT_FAILURE);
      }
    }

    else {
      while(pendurados2 != tinfo->trab) {
        if(pthread_cond_wait(&cond2, &mutex2) != 0) {
          fprintf(stderr, "\nErro ao esperar pela variável de condição\n");
          exit(EXIT_FAILURE);
        }
      }
    }

    if(pthread_mutex_unlock(&mutex2) != 0) {
      fprintf(stderr, "\nErro ao desbloquear mutex\n");
      exit(EXIT_FAILURE);
    }

    // todas as escravas abortam a execucao
    if(aborta)
      return 0;
  }

  return 0;
}


/*--------------------------------------------------------------------
| Function: parse_integer_or_exit
---------------------------------------------------------------------*/

int parse_integer_or_exit(char const *str, char const *name)
{
  int value;

  if(sscanf(str, "%d", &value) != 1) {
    fprintf(stderr, "\nErro no argumento \"%s\".\n\n", name);
    exit(1);
  }
  return value;
}


/*--------------------------------------------------------------------
| Function: parse_double_or_exit
---------------------------------------------------------------------*/

double parse_double_or_exit(char const *str, char const *name)
{
  double value;

  if(sscanf(str, "%lf", &value) != 1) {
    fprintf(stderr, "\nErro no argumento \"%s\".\n\n", name);
    exit(1);
  }
  return value;
}


/*--------------------------------------------------------------------
| Function: main
---------------------------------------------------------------------*/

int main (int argc, char** argv) {
  if(argc != 9) {
    fprintf(stderr, "\nNumero invalido de argumentos.\n");
    fprintf(stderr, "Uso: heatSim N tEsq tSup tDir tInf iteracoes trab maxD\n\n");
    return 1;
  }

  /* argv[0] = program name */
  int N = parse_integer_or_exit(argv[1], "N");
  double tEsq = parse_double_or_exit(argv[2], "tEsq");
  double tSup = parse_double_or_exit(argv[3], "tSup");
  double tDir = parse_double_or_exit(argv[4], "tDir");
  double tInf = parse_double_or_exit(argv[5], "tInf");
  int iteracoes = parse_integer_or_exit(argv[6], "iteracoes");
  int trab = parse_integer_or_exit(argv[7], "trab");
  int maxD = parse_integer_or_exit(argv[8], "maxD");

  DoubleMatrix2D *matrix, *matrix_aux;
  int i, res;


  fprintf(stderr, "\nArgumentos:\n"
   " N=%d tEsq=%.1f tSup=%.1f tDir=%.1f tInf=%.1f iteracoes=%d trab=%d maxD=%d\n",
   N, tEsq, tSup, tDir, tInf, iteracoes, trab, maxD);

  if(N < 1 || tEsq < 0 || tSup < 0 || tDir < 0 || tInf < 0 || iteracoes < 1) {
    fprintf(stderr, "\nErro: Argumentos invalidos.\n"
  " Lembrar que N >= 1, temperaturas >= 0 e iteracoes >= 1\n\n");
    return 1;
  }

  matrix = dm2dNew(N+2, N+2);
  matrix_aux = dm2dNew(N+2, N+2);

  if (matrix == NULL || matrix_aux == NULL) {
    fprintf(stderr, "\nErro: Nao foi possivel alocar memoria para as matrizes.\n\n");
    return -1;
  }

  if ((N%trab) != 0)
    return -1;

  dm2dSetLineTo (matrix, 0, tSup);
  dm2dSetLineTo (matrix, N+1, tInf);
  dm2dSetColumnTo (matrix, 0, tEsq);
  dm2dSetColumnTo (matrix, N+1, tDir);

  dm2dCopy (matrix_aux, matrix);

  /* Reservar Memória para Trabalhadoras */
  thread_info *tinfo = (thread_info *)malloc(trab * sizeof(thread_info));
  pthread_t *trabalhadoras = (pthread_t *)malloc(trab * sizeof(pthread_t));

  if (tinfo == NULL || trabalhadoras == NULL) {
    fprintf(stderr, "\nErro ao alocar memória para trabalhadoras.\n");
    return -1;
  }


  /* Inicializar mutexes e variaveis de condicao */
  if(pthread_mutex_init(&mutex, NULL) != 0) {
    fprintf(stderr, "\nErro ao inicializar mutex\n");
    return -1;
  }

  if(pthread_mutex_init(&mutex2, NULL) != 0) {
    fprintf(stderr, "\nErro ao inicializar mutex\n");
    return -1;
  }

  if(pthread_mutex_init(&mutex3, NULL) != 0) {
    fprintf(stderr, "\nErro ao inicializar mutex\n");
    return -1;
  }

  if(pthread_cond_init(&cond, NULL) != 0) {
    fprintf(stderr, "\nErro ao inicializar variável de condição\n");
    return -1;
  }

  if(pthread_cond_init(&cond2, NULL) != 0) {
    fprintf(stderr, "\nErro ao inicializar variável de condição\n");
    return -1;
  }


  /* Criar Trabalhadoras */
  for (i = 0; i < trab; i++) {
    tinfo[i].id = i+1;
    tinfo[i].N = N;
    tinfo[i].iter = iteracoes;
    tinfo[i].trab = trab;
    tinfo[i].tam_fatia = N/trab;
    tinfo[i].matrixes[0] = matrix;
    tinfo[i].matrixes[1] = matrix_aux;
    tinfo[i].maxD = maxD;
    tinfo[i].inicio = i*N/trab + 1;
    tinfo[i].fim = i*N/trab + 1 + N/trab;
    res = pthread_create(&trabalhadoras[i], NULL, tarefa_trabalhadora, &tinfo[i]);

    if(res != 0) {
      fprintf(stderr, "\nErro ao criar uma tarefa trabalhadora.\n");
      return -1;
    }
  }

  /* Esperar que as Trabalhadoras Terminem */
  for (i = 0; i < trab; i++) {
    res = pthread_join(trabalhadoras[i], NULL);

    if (res != 0) {
      fprintf(stderr, "\nErro ao esperar por uma tarefa trabalhadora.\n");
      return -1;
    }
  }

  if (iteracoesEfetuadas % 2)
    dm2dPrint(matrix);
  else
    dm2dPrint(matrix_aux);


  /* Destruicao de mutexes e variaveis de condicao */

  if(pthread_mutex_destroy(&mutex) != 0) {
    fprintf(stderr, "\nErro ao destruir mutex\n");
    exit(EXIT_FAILURE);
  }

  if(pthread_mutex_destroy(&mutex2) != 0) {
    fprintf(stderr, "\nErro ao destruir mutex\n");
    exit(EXIT_FAILURE);
  }

  if(pthread_mutex_destroy(&mutex3) != 0) {
    fprintf(stderr, "\nErro ao destruir mutex\n");
    exit(EXIT_FAILURE);
  }

  if(pthread_cond_destroy(&cond) != 0) {
    fprintf(stderr, "\nErro ao destruir variável de condição\n");
    exit(EXIT_FAILURE);
  }

  if(pthread_cond_destroy(&cond2) != 0) {
    fprintf(stderr, "\nErro ao destruir variável de condição\n");
    exit(EXIT_FAILURE);
  }

  free(tinfo);
  free(trabalhadoras);
  dm2dFree(matrix);
  dm2dFree(matrix_aux);

  return 0;
}
