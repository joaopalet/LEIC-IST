#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define GAVETAS 16
#define MAX_KEY 8
#define strings_iguais(X, Y) (!strcmp(X,Y))
#define stock(A) A->stock
#define chave(A) A->chave
#define produto(A) A->produto
#define next(A) A->next
#define prev(A) A->prev


						
typedef struct {
	char chave[MAX_KEY+1];
	int stock;
} Produto;

typedef Produto* item;

typedef struct node{
	item produto;
	struct node* next;
	struct node* prev;
} Node;

typedef Node* link;

link armazem[GAVETAS][GAVETAS][GAVETAS];

static int tipos = 0;	/* Numero de produtos diferentes no armazem */

static item pmax = NULL;	/* Ponteiro que aponta para o produto com maior stock */



/*	FUNCOES AUXILIARES	*/

void Init() {
	int i, j, m;
	for (i = 0; i < GAVETAS; i++)
		for (j = 0; j < GAVETAS; j++) 
			for (m = 0; m < GAVETAS; m++)
				armazem[i][j][m] = NULL;
}

int hash(char codigo[], int n){
	if((int) codigo[n] <= 57 && (int) codigo[n] >= 48)
		return ((int) codigo[n])-48;
	else{	
		int aux;
		aux = ((int) codigo[n]) - 87;
		return aux;
	}
}

int compara_chaves(item item1, item item2){		/* Retorna 1 quando: chave item1 < chave item2 */
	if(strcmp(chave(item1), chave(item2)) < 0)
		return 1;
	else
		return 0;
	}

link NEW(char c[], int n){
	link aux = (link) malloc(sizeof(Node));
	produto(aux) = (item) malloc(sizeof(Produto));
 	
 	strcpy(chave(produto(aux)), c);
 	
 	if (n>=0)
 		stock(produto(aux)) = n;
 	else
 		stock(produto(aux)) = 0;
 	
 	next(aux) = NULL;
 	
 	return aux; 
 }

int verifica_max(item produto){		/* Retorna 1 quando o produto passa a ser o que tem o stock maximo */
	if(pmax != NULL){
		if (stock(produto) > stock(pmax))
			return 1;
		else if (stock(produto) == stock(pmax) && compara_chaves(produto, pmax))
			return 1;
	}
	
	return 0;
}

void insere_ou_altera(char key[], int x, int y, int z, int num){
	link i, novo, aux1, aux2;
	int compara;

	if(armazem[x][y][z] == NULL){		/* Quando a lista esta vazia */
		novo = NEW(key, num);
		armazem[x][y][z] = novo;
		prev(novo) = armazem[x][y][z];
		tipos++;
		
		if(verifica_max(produto(novo)))
			pmax = produto(novo);
		return;
	}
	for(i = armazem[x][y][z]; i != NULL; i = next(i)){
		aux1 = i;
		compara = strcmp(key, chave((produto(i))));

		if(compara == 0){		/* Quando o produto ja' existe */
			if(produto(i) == pmax && num < 0)	/* Caso em que se removem elementos do atual maximo */
				pmax = NULL;

			if (stock(produto(i)) + num >= 0)
				stock(produto(i)) += num;
			else
				stock(produto(i)) = 0;
			
			if(verifica_max(produto(i)))	/* Quando o produto atualizado fica com mais unidades em stock que o maximo */
				pmax = produto(i);

			return;
		}

		if(compara < 0){		/* Quando o produto nao existe, e temos de adicionar um novo */
			tipos++;
			novo = NEW(key, num);
			
			if(verifica_max(produto(novo)))
				pmax = produto(novo);

			if(i==armazem[x][y][z]){		/* No caso em que se insere na primeira posicao da lista */
				aux2 = i;
				armazem[x][y][z] = novo;
				prev(novo) = armazem[x][y][z];
				next(novo) = i;
				prev(i) = novo;
				return;
			}
			
			aux2 = prev(i);		/* No caso que se adiciona entre dois elementos da lista */
			prev(i) = novo;
			next(novo) = i;
			next(aux2) = novo;
			prev(novo) = aux2;
			return;
		}
	}
	
	/* Quando se insere um produto no fim da lista */ 
	novo = NEW(key, num);

	if(verifica_max(produto(novo)))
		pmax = produto(novo);

	next(aux1) = novo;
	prev(novo) = aux1;
	tipos++;
}

link procura(int x, int y, int z, char key[]){
	link i;

	for(i = armazem[x][y][z]; i != NULL; i = next(i))
		if(strings_iguais(key, (chave(produto(i)))))
			return i;

	return NULL;
}

void apaga_elemento(int x, int y, int z, link bloco){
	if(bloco == armazem[x][y][z]){		/* Quando se remove o primeiro elemento da lista */
		armazem[x][y][z] = (next(bloco));
		prev(next(bloco)) = armazem[x][y][z];
	}
	
	else if(next(bloco) == NULL)	/* Quando o bloco a remover e' o ultimo Node da lista */
		next(prev(bloco)) = NULL;

	else{	/* Quando se remove um Node no meio da lista */
		link anterior = prev(bloco);
		next(anterior) = next(bloco);
		prev(next(bloco)) = anterior;
	}

	free(produto(bloco));
	free(bloco);
}

void free_all(){
	int i, j, m;
	link aux, proximo;

	for (i = 0; i < GAVETAS; i++)
		for (j = 0; j < GAVETAS; j++)
			for (m = 0; m < GAVETAS; m++)
				for (aux = armazem[i][j][m]; aux != NULL; aux = proximo){
					proximo = next(aux);
					free(produto(aux));
					free(aux);
				}
}

void imprime_ordenado(){
	int i, j, m;
	link aux1;
	item aux2;

	for (i = 0; i < GAVETAS; i++)
		for (j = 0; j < GAVETAS; j++)
			for (m = 0; m < GAVETAS; m++)
				for (aux1 = armazem[i][j][m]; aux1 != NULL; aux1 = next(aux1)){
					aux2 = produto(aux1);
					printf("%s %d\n", chave(aux2), stock(aux2));
				}
}

void encontra_max(){
	int i, j, m, max = -1;
	link aux;

	for (i = 0; i < GAVETAS; i++)
		for (j = 0; j < GAVETAS; j++)
			for (m = 0; m < GAVETAS; m++)
				for (aux = armazem[i][j][m]; aux != NULL; aux = next(aux))
					if (stock(produto(aux)) > max){	/*Quando tem maior stock que o maximo passa a maximo */
						max = stock(produto(aux));
						pmax = produto(aux);
					}
					else if((stock(produto(aux)) == max) && compara_chaves(produto(aux), pmax)){	/* Quando tem o mesmo numero de unidades mas a chave e' lexicograficamente menor, passa a maximo */
						max = stock(produto(aux));
						pmax = produto(aux);
					}
}

void apaga(int x, int y, int z, link caixa){
	if(caixa == NULL)		/* Quando o elemento a remover nao existe */
		return;
	else{
		tipos--;

		if(produto(caixa) == pmax)  /* Quando e' removido o maior */
			pmax = NULL;

		if(caixa == armazem[x][y][z] && next(caixa) == NULL){		/* Quando so existe um Node em armazem[x][y] */
			armazem[x][y][z] = NULL;
			free(produto(caixa));
			free(caixa);
		}
		
		else
			apaga_elemento(x, y, z, caixa);
	}
}



/* FUNCOES DO MENU */

void executa_a(){

	int x, y, z, num;
	char chave[MAX_KEY+1];

	scanf(" %s ", chave);
	scanf("%d", &num);
	getchar();

	x = hash(chave, 0);
	y = hash(chave, 1);
	z = hash(chave, 2);

	insere_ou_altera(chave, x, y, z, num);
}

void executa_l(){
	imprime_ordenado();
}

void executa_m(){
	if(pmax == NULL)
		encontra_max();
	
	if(pmax != NULL)
		printf("%s %d\n", chave(pmax), stock(pmax));
}

void executa_r(){
	char chave[MAX_KEY+1];
	int x, y, z;

	scanf(" %s", chave);

	x = hash(chave, 0);
	y = hash(chave, 1);
	z = hash(chave, 2);

	link aux = procura(x, y, z, chave);
	
	apaga(x, y, z, aux);
}


void executa_x(){
	printf("%d\n", tipos);
	free_all();
}




/* MAIN */

int main()
{
	char command;
	Init();
 	
 	while (1) 
 	{
 		command = getchar(); /* Le o comando */

 		switch (command) 
 		{
 		
 		case 'a':
 			executa_a();
 			break;
 		
 		case 'l':
 			executa_l();
 			getchar();
 			break;

 		case 'm':
 			executa_m();
 			getchar();
 			break;

 		case 'r':
 			executa_r();
 			getchar();
 			break;

 		case 'x':
 			executa_x();
 			getchar();
 			return 0; /* Termina o programa com sucesso */
 		
 		default:
 			printf("ERRO: Comando desconhecido\n");
 		}
 	}

	return 0; /* Se chegou aqui algo correu mal */
}