/* Francisco Sena - 86420 */
/* João Almeida - 85447 */
/* G63 - Alameda */


#include <stdio.h>
#include <string.h>

#define MAXUSERS 1000
#define MAXMSGS	10000
#define MAXFRASE 141
#define exch(A, B) { int t = A; A = B; B = t; }



typedef struct {
	int id;
	char text[MAXFRASE];
} Mensagem;



Mensagem forum[MAXMSGS];
int numMsgs = 0;



void leLinha(char s[])   /* funcao auxiliar que le uma linha do standard input */
{
	char c;
	int i = 0;

	while ((c = getchar()) != '\n')
	{
		s[i] = c;
		i++;
	}

	s[i] = '\0';
}



int conta_palavra(char s[], char p[], int c)
{
	int i, j;

	i=c;

	for (j=0; p[j] != '\0'; j++)
		if (s[i+j] != p[j])		/* quando uma das letras difere, a funcao termina logo pois as palavras nao sao iguais */
			return 0;

	if(s[i+j] == ' ' || s[i+j] == ',' || s[i+j] == '\t' || s[i+j] == ';' || s[i+j] == '.' || s[i+j] == '\0')	/* se apos todas as letras das palavras serem iguais, verifica se estamos mesmo perante o fim de uma palavra e nao a meio */
		return 1;

	return 0;
}





/* FUNCOES DO MENU */

void executa_A()
{
	scanf("%d ", &forum[numMsgs].id);
	leLinha(forum[numMsgs].text);

	numMsgs++;
}



void executa_L()
{
	int i;

	printf("*TOTAL MESSAGES:%d\n", numMsgs);
	for (i = 0; i < numMsgs; i++)
		printf("%d:%s\n", forum[i].id, forum[i].text);
}



void executa_U()
{
	int i;
	int user;

	scanf("%d", &user);
	printf("*MESSAGES FROM USER:%d\n", user);
	for (i = 0; i < numMsgs; i++)
		if (forum[i].id == user)
			printf("%s\n", forum[i].text);
}



void executa_O()
{
	int i;
	
	int maximo = 0;
	for (i = 0; i < numMsgs; i++)
		if (strlen(forum[i].text) > maximo)
			maximo = strlen(forum[i].text);    
	for (i = 0; i < numMsgs; i++)
		if (strlen(forum[i].text) == maximo)
			printf("*LONGEST SENTENCE:%d:%s\n", forum[i].id, forum[i].text);
}



void executa_T()
{
	int i, max = 0, conta[MAXUSERS] = {0};


	for(i = 0; i < numMsgs; i++)   /* guardar o numero de mensagens de cada utilizador na respetiva posicao do vetor auxiliar 'conta' (ex: conta[53] = 3, user 53 tem 3 mensagens no forum) */
		conta[forum[i].id]++;
	for(i = 0; i < MAXUSERS; i++)  /* de todos as mensagens do forum, procura o maior numero de mensangens escritas por um utilizador */
		if(conta[i] > max)
			max = conta[i];
	for(i = 0; i < MAXUSERS; i++)  
		if(conta[i] == max)   		/* quando ha mais do que um utilizador com "max" numero de mensagens */
			printf("*MOST ACTIVE USER:%d:%d\n", i, max);
}



void executa_C()
{
	int i, k, n_palavras = 0;
	char p[MAXFRASE];

	scanf(" %s", p);

	for(k = 0; k < numMsgs; k++)
	{
		if(forum[k].text[0] == p[0])	/* quando a primeira letra da mensagem é igual à primeira da palavra */
			n_palavras += conta_palavra(forum[k].text, p, 0);	/* conta_palavra retorna 1 se a palavra for igual a palavra p */

		for(i = 0; forum[k].text[i] != '\0'; i++)	
			if((forum[k].text[i] == ' ' || forum[k].text[i] == ',' || forum[k].text[i] == '\t' || forum[k].text[i] == ';' || forum[k].text[i] == '.') && forum[k].text[i+1] == p[0]) /* quando encontra o inicio de uma palavra (ex: ',', ';', ' ', '.', '\t' e a letra seguinte e a primeira letra da palavra p */
				n_palavras += conta_palavra(forum[k].text, p, i+1);
	}

	printf("*WORD %s:%d\n", p, n_palavras);
}



void executa_S()
{
	int aux[numMsgs], a, min, j, k;

	printf("*SORTED MESSAGES:%d\n", numMsgs);

	for(a = 0; a < numMsgs; a++)	/* inicicalizacao do vetor auxiliar por ordem dos indices */
		aux[a] = a;

	for(j = 0; j < numMsgs; j++)   						/* Selection Sort aplicado as mensagens do forum */
	{
		min = j;
		for(k = j+1; k < numMsgs; k++)
		{
			if(strcmp(forum[aux[min]].text, forum[aux[k]].text) > 0)	/* compara lexicograficamente a "menor" mensagem com outra */
				min = k;                                                    /* atualiza o indice da "menor" frase */
			else if(strcmp(forum[aux[min]].text, forum[aux[k]].text) == 0)		/* situacao em que as mensagens sao iguais */
			{
				if (forum[aux[k]].id < forum[aux[min]].id) 
					min = k;
				else if(k < min)
					min = k;
			}
		}
		exch(aux[j], aux[min]);      /* no final de cada iteracao, troca o indice do vetor auxiliar cuja frase e a "menor" com indice j do vetor auxiliar */ 
	}

	for(a = 0; a < numMsgs; a++) 
		printf("%d:%s\n", forum[aux[a]].id, forum[aux[a]].text);    
}



void executa_X()
{
	printf("%d\n", numMsgs);
}





/* MAIN */

int main()
{
	char command;
 	
 	while (1) 
 	{
 		command = getchar(); /* Le o comando */

 		switch (command) 
 		{
 		
 		case 'A':
 			executa_A();
 			break;
 		
 		case 'L':
 			executa_L();
 			getchar();  /* Se em executa_L NAO lemos o \n, temos de o ler aqui. */
 			break;

 		case 'U':
 			executa_U();
 			getchar();  /* Se em executa_U NAO lemos o \n, temos de o ler aqui. */
 			break;

 		case 'O':
 			executa_O();
 			getchar();  /* Se em executa_O NAO lemos o \n, temos de o ler aqui. */
 			break;

 		case 'T':
 			executa_T();
 			getchar();  /* Se em executa_T NAO lemos o \n, temos de o ler aqui. */
 			break;

 		case 'C':
 			executa_C(); /* Se em executa_C NAO lemos o \n, temos de o ler aqui. */
 			getchar();
 			break;

 		case 'S':
 			executa_S(); /* Se em executa_S NAO lemos o \n, temos de o ler aqui. */
 			getchar();
 			break;
 		
 		case 'X':
 			executa_X();
 			return 0; /* Termina o programa com sucesso */
 		
 		default:
 			printf("ERRO: Comando desconhecido\n");
 		}
 	}

	return -1; /* Se chegou aqui algo correu mal */
}