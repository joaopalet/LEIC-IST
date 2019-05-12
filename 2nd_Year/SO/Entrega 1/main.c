/*
Projeto SO - exercicio 1

Francisco Sena - 86420
Joao Almeida - 86447
*/



#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

#include "mplib3.h"
#include "leQueue.h"
#include "matrix2d.h"

#define     getThreadID(args)   args->id
#define     getNumSlaves(args)  args->n
#define     getFatia(args)      args->fatias
#define     getColunas(args)    args->n_colunas
#define     getIteracoes(args)  args->iteracoes





typedef struct {
  int id;
  int n;
  int fatias;
  int n_colunas;
  int iteracoes;
}args_mythread_t;





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
| Function: slaveThread
---------------------------------------------------------------------*/

void *slaveThread(void *a){
  args_mythread_t *args = (args_mythread_t *) a;
  DoubleMatrix2D *tmp;
  int i, j;
  double value;

  if(getFatia(args) <= 2 || getColunas(args) <= 2)
    return NULL;

  DoubleMatrix2D *mini_matrix = dm2dNew(getFatia(args), getColunas(args));
  DoubleMatrix2D *mini_matrix_aux = dm2dNew(getFatia(args), getColunas(args));

  double *linha = (double*) malloc(sizeof(double)*getColunas(args));
  if(linha == NULL)
    return NULL;

  if (mini_matrix == NULL || mini_matrix_aux == NULL)
    return NULL;

  /*  Rececao da respetiva fatia, enviada pela tarefa mestre  */
  for(i = 0; i < getFatia(args); i++) {
    receberMensagem(0, getThreadID(args), linha, sizeof(double)*getColunas(args));
    dm2dSetLine(mini_matrix, i, linha);
  }

  dm2dCopy (mini_matrix_aux, mini_matrix);

  double *linha_em_falta1 = (double *) malloc(sizeof(double)*(getColunas(args)));
  if (linha_em_falta1 == NULL)
    return NULL;

  double *linha_em_falta2 = (double *) malloc(sizeof(double)*(getColunas(args)));
  if (linha_em_falta2 == NULL)
    return NULL;

  //  Calculo dos valores das entradas (algorimo "simul")
  for (int k = 0; k < getIteracoes(args); k++){
    for (i = 1; i < getFatia(args) - 1; i++){
      for (j = 1; j < getColunas(args) - 1; j++){
        value = ( dm2dGetEntry(mini_matrix, i-1, j) + dm2dGetEntry(mini_matrix, i+1, j) + dm2dGetEntry(mini_matrix, i, j-1) + dm2dGetEntry(mini_matrix, i, j+1) ) / 4.0;

        dm2dSetEntry(mini_matrix_aux, i, j, value);
      }
    }



    /*  Troca de mensagens entre as threads escravas  */

    //  Thread responsavel pela fatia superior so envia mensagem para a thread seguinte
    if (getThreadID(args) != 1){
        enviarMensagem(getThreadID(args), getThreadID(args) - 1, dm2dGetLine(mini_matrix_aux, 1), sizeof(double)*(getColunas(args)));
        receberMensagem(getThreadID(args) - 1, getThreadID(args), linha_em_falta1, sizeof(double)*(getColunas(args)));

        dm2dSetLine (mini_matrix_aux, 0, linha_em_falta1);
    }

    //  Thread responsavel pela fatia inferior so envia mensagem para a thread anterior
    if (getThreadID(args) != getNumSlaves(args)){
        receberMensagem(getThreadID(args) + 1, getThreadID(args), linha_em_falta2, sizeof(double)*(getColunas(args)));
        enviarMensagem(getThreadID(args), getThreadID(args) + 1, dm2dGetLine(mini_matrix_aux, mini_matrix_aux->n_l - 2), sizeof(double)*(getColunas(args)));

        dm2dSetLine (mini_matrix_aux, mini_matrix_aux->n_l - 1, linha_em_falta2);
    }

    tmp = mini_matrix_aux;
    mini_matrix_aux = mini_matrix;
    mini_matrix = tmp;
  }

  enviarMensagem(getThreadID(args), 0, mini_matrix->data, sizeof(double) * getColunas(args) * getFatia(args));

  dm2dFree(mini_matrix);
  dm2dFree(mini_matrix_aux);
  free(linha);
  free(linha_em_falta1);
  free(linha_em_falta2);

  return 0;
}



/*--------------------------------------------------------------------
| Function: main
---------------------------------------------------------------------*/

int main (int argc, char** argv){

  if(argc != 9){
    fprintf(stderr, "\nNumero invalido de argumentos.\n");
    fprintf(stderr, "Uso: heatSim N tEsq tSup tDir tInf iteracoes trab csz\n\n");
    return 1;
  }

  int N = parse_integer_or_exit(argv[1], "N");
  double tEsq = parse_double_or_exit(argv[2], "tEsq");
  double tSup = parse_double_or_exit(argv[3], "tSup");
  double tDir = parse_double_or_exit(argv[4], "tDir");
  double tInf = parse_double_or_exit(argv[5], "tInf");
  int iteracoes = parse_integer_or_exit(argv[6], "iteracoes");
  int trab = parse_integer_or_exit((argv[7]), "trab");
  int csz = parse_integer_or_exit(argv[8], "csz");

  DoubleMatrix2D *matrix = dm2dNew(N+2, N+2);
  dm2dSetLineTo (matrix, 0, tSup);
  dm2dSetLineTo (matrix, N+1, tInf);
  dm2dSetColumnTo (matrix, 0, tEsq);
  dm2dSetColumnTo (matrix, N+1, tDir);

  if (inicializarMPlib(csz, trab+1) == -1)
    return -1;

  fprintf(stderr, "\nArgumentos:\n"
  " N=%d tEsq=%.1f tSup=%.1f tDir=%.1f tInf=%.1f iteracoes=%d trab=%d czs=%d\n",
  N, tEsq, tSup, tDir, tInf, iteracoes, trab, csz);

  if(N < 1 || tEsq < 0 || tSup < 0 || tDir < 0 || tInf < 0 || iteracoes < 1) {
    fprintf(stderr, "\nErro: Argumentos invalidos.\n"
    " Lembrar que N >= 1, temperaturas >= 0 e iteracoes >= 1\n\n");
    return 1;
  }

  if ((N%trab) != 0)
    return -1;

  int i, j, k;

  args_mythread_t *slave_args = (args_mythread_t*)malloc(sizeof(args_mythread_t)*trab);
  pthread_t *slaves = (pthread_t*) malloc(trab*sizeof(pthread_t));

  if (slave_args == NULL || slaves == NULL)
    return 1;

  for (i=0; i < trab; i++) {
    slave_args[i].id = i+1;
    slave_args[i].n = trab;
    slave_args[i].fatias = N/trab + 2;
    slave_args[i].n_colunas = N + 2;
    slave_args[i].iteracoes = iteracoes;

    if(pthread_create(&slaves[i], NULL, slaveThread, &slave_args[i]) != 0)
      return -1;
  }



  /*  Envio para as threads das respetivas fatias iniciais */

  for(i = 1, j = 0; i <= trab; i++) {
      for(k = 0; k < N/trab + 2; k++, j++) {
          enviarMensagem(0, i, dm2dGetLine(matrix, j), sizeof(double) * matrix->n_c);
      }
      j -= 2;
  }

  DoubleMatrix2D *mini_matrix = dm2dNew(N/trab + 2, N + 2);
  if(mini_matrix == NULL)
    return -1;



  /*  Rececao das fatias das threads e colocacao das mesmas na matriz final */

  for (i = 1, k = 1; i <= trab; i++) {
    receberMensagem(i, 0, mini_matrix->data, sizeof(double) * (N + 2) * (N/trab + 2));

    for (j = 1; j < (N/trab + 2) - 1; j++, k++) {
        dm2dSetLine(matrix, k, dm2dGetLine(mini_matrix, j));
    }
  }

  for (i = 0; i < trab; i++) {
    if (pthread_join(slaves[i], NULL)) {
      fprintf(stderr, "\nErro ao esperar por um escravo.\n");
      return -1;
    }
  }

  dm2dPrint(matrix);

  free(slaves);
  free(slave_args);
  dm2dFree(matrix);
  dm2dFree(mini_matrix);
  libertarMPlib();

  return 0;
}
