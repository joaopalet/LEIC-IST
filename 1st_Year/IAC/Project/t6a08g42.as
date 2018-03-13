
;-----------------------------Definicao de constantes--------------------------------

CR              EQU     0Ah
IO_READ         EQU     FFFFh
IO_WRITE        EQU     FFFEh
IO_STATUS       EQU     FFFDh
IO_CURSOR		EQU		FFFCh
IO_TIME			EQU		FFF6h
IO_TIME_ON		EQU		FFF7h
ENABLE_TIMER	EQU		0001h
SP_INICIAL      EQU     FDFFh
INT_MASK_ADDR   EQU     FFFAh
INT_MASK        EQU     1111111111111111b
INT_MASK_RESET	EQU		0111111111111111b
HASH			EQU		'#'
CANHAO			EQU		'>'
CORPO_NAVE		EQU		')'
TOPO_NAVE		EQU		'\'
BAIXO_NAVE		EQU		'/'
ASTEROIDE		EQU		'*'
BURACO_NEG		EQU		'O'
TIRO			EQU		'-'
LIMPA			EQU		' '
FIM_TEXTO		EQU		'@'
MASCARA_RAND	EQU		1000000000010110b
LEDS			EQU		FFF8h
CONTROLO_LCD	EQU		FFF4h
ESCRITA_LCD		EQU		FFF5h
DISPLAY1		EQU 	FFF0h
DISPLAY2		EQU 	FFF1h
DISPLAY3		EQU 	FFF2h
DISPLAY4		EQU 	FFF3h

				

				
;----------------------------Definicao de variaveis-----------------------------------

				ORIG	8000h
Mensagem		STR		'Prima a tecla IE para iniciar o jogo', FIM_TEXTO
BOTAOPRESS		WORD	0
POS_CANHAO		WORD	0503h
POS_CORPO_NAVE	WORD	0502h
POS_TOPO_NAVE	WORD	0402h
POS_BAIXO_NAVE	WORD	0602h
POS_TIRO		WORD	0602h
MOVE_CIMA		WORD	0000h
MOVE_DIREITA	WORD	0000h
MOVE_ESQUERDA	WORD	0000h
MOVE_BAIXO		WORD	0000h
DEU_TIRO		WORD	0000h
ATUALIZA_TIRO	WORD	0000h
P_ASTEROIDE		TAB		17
P_BURACO		TAB		5
CRIA_AST		WORD	000Ah
NINI			WORD	0008h
ATUALIZA_AST	WORD	000Ch
LINHA_AST		WORD	0000h
VEZ_BURACO		WORD	0003h
SCORE			WORD	0000h
GAME_OVER		WORD	0001h
MSG_FIM_JOGO	STR		'Fim do Jogo',	FIM_TEXTO	
MSG_PONTUACAO	STR		'Pontuacao: ',	FIM_TEXTO
MSG_RESET		STR		'Prima qualquer botao para reiniciar o R-TYPE',	FIM_TEXTO
RESET			WORD	0001h		
LINHA			STR		'Lin: ', FIM_TEXTO
COLUNA			STR		'Col: ', FIM_TEXTO	
				
				
				
				
;----------------------------Tabela de interrupcoes-----------------------------------		
				
				ORIG	FE00h
				
INT0			WORD	INT_Baixo
INT1			WORD	INT_Cima
INT2			WORD 	INT_Esquerda
INT3			WORD	INT_Direita
INT4			WORD	INT_Tiro
INT5			WORD	INT_Reset
INT6			WORD	INT_Reset
INT7			WORD	INT_Reset
INT8			WORD	INT_Reset
INT9			WORD	INT_Reset
INT10			WORD	INT_Reset
INT11			WORD	INT_Reset
INT12			WORD	INT_Reset
INT13			WORD	INT_Reset
INT14			WORD	INT_Iniciar	
INT15			WORD	INT_Tempo




;----------------------------Codigo---------------------------------------------------
				
				ORIG	0000h
				JMP 	Inicio


				
				
;----------------------------Tratamentos----------------------------------------------				
;	Acoes geradas pelas interrupcoes

INT_Baixo:		PUSH	1h
				POP		M[MOVE_BAIXO]
				MOV		M[RESET], R0
				RTI

INT_Cima:		PUSH	1h
				POP		M[MOVE_CIMA]
				MOV		M[RESET], R0
				RTI

INT_Esquerda:	PUSH	1h
				POP		M[MOVE_ESQUERDA]
				MOV		M[RESET], R0
				RTI

INT_Direita:	PUSH	1h
				POP		M[MOVE_DIREITA]
				MOV		M[RESET], R0
				RTI

INT_Tiro:		PUSH	1h
				POP		M[DEU_TIRO]
				MOV		M[RESET], R0
				RTI

INT_Iniciar:	MOV		R5,	1h
				MOV		M[RESET], R0
				RTI
				
INT_Tempo:		DEC		M[ATUALIZA_TIRO]
				DEC		M[CRIA_AST]
				DEC		M[ATUALIZA_AST]
				PUSH	ENABLE_TIMER
				POP		M[IO_TIME_ON]
				PUSH	1h
				POP		M[IO_TIME]
				RTI
				
INT_Reset:		MOV		M[RESET], R0
				RTI
	
	
	
	
;----------------------------Movimentos----------------------------------------------				
;	Movimentos ativados pelas interrupcoes

MoveCima:		PUSH	0h
				POP		M[MOVE_CIMA]
				CALL 	ApagaNave
				MOV		R5,	FF00h
				MOV		R1, R0
				MVBH	R1,	M[POS_TOPO_NAVE]
				ADD		R1,	R5
				CMP		R1, 0000h
				CALL.NZ	Movimento
				CALL	DesNave
				CALL	EscreveLCD
				RET

MoveDireita:	PUSH	0h
				POP		M[MOVE_DIREITA]
				CALL 	ApagaNave
				MOV		R5,	0001h
				MOV		R1, R0
				MVBL	R1,	M[POS_CANHAO]
				ADD		R1, R5
				CMP		R1,	0050h
				CALL.NZ	Movimento
				CALL	DesNave
				CALL	EscreveLCD
				RET

MoveEsquerda:	PUSH	0h
				POP		M[MOVE_ESQUERDA]
				CALL 	ApagaNave
				MOV		R5,	FFFFh
				MOV		R1, R0
				MVBL	R1,	M[POS_CORPO_NAVE]
				CMP		R1,	0000h
				CALL.NZ	Movimento
				CALL	DesNave
				CALL	EscreveLCD
				RET

MoveBaixo:		PUSH	0h
				POP		M[MOVE_BAIXO]
				CALL 	ApagaNave
				MOV		R5,	0100h
				MOV		R1, R0
				MVBH	R1,	M[POS_BAIXO_NAVE]
				ADD		R1,	R5
				CMP		R1, 1700h
				CALL.NZ	Movimento
				CALL	DesNave
				CALL	EscreveLCD
				RET               

				
	
	
;----------------------------Tiro-----------------------------------------------------
;	DeuTiro:		Rotina ativada quando o utilizador prime I4. Imprime um tiro
;					na janela de texto na posicao a frente do canhao da nave

;	AtualizaTiro:	Rotina que apaga o tiro e imprime-o uma posicao a frente quando
;					o temporizador chega a 0

DeuTiro:		CALL	ApagaTiro
				MOV		M[DEU_TIRO], R0			;Reposicao da flag
				MOV		R2,	M[POS_CANHAO]
				MOV		M[POS_TIRO], R2
				INC		M[POS_TIRO]
				PUSH	M[POS_TIRO]
				POP		M[IO_CURSOR]
				PUSH	TIRO
				POP		M[IO_WRITE]
				PUSH	2h
				POP		M[ATUALIZA_TIRO]
				RET

IncrementaTiro:	MOV		R4,	0001h
				ADD		M[POS_TIRO],	R4
				CALL	DesTiro
				RET				
AtualizaTiro:	PUSH	2h
				POP		M[ATUALIZA_TIRO]
				MOV		M[LEDS],	R0
				CALL    ApagaTiro
				DSI
				CALL	ComparaA				;Compara a posicao do tiro com a dos asteroides
				CALL	ComparaB				;Compara a posicao do tiro com a dos buracos negros
				ENI
				MOV		R6,	R0
				MVBL	R6,	M[POS_TIRO]
				CMP		R6, 0050h
				CALL.NP	IncrementaTiro
				DSI
				CALL	ComparaA				;Compara a posicao do tiro com a dos asteroides
				CALL	ComparaB				;Compara a posicao do tiro com a dos buracos negros
				ENI
				RET
				
ComparaA:		MOV		R5,	P_ASTEROIDE
				MOV		R4,	M[POS_TIRO]
				CMP		M[R5],	R4
				BR.Z	MataAst
Incrementa3:	INC		R5
				CMP		R5,	8042h				;Verifica se ja atingiu o ultimo endereco do vetor
				BR.Z	Retorna2
				CMP		M[R5],	R4
				BR.NZ	Incrementa3
MataAst:		CALL	ApagaTiro
				PUSH	1851h
				POP		M[POS_TIRO]
				CALL	ApagaAst				
				MOV		M[R5],	R0				;Limpa/liberta (coloca a 0000h) o conteudo do endereco de memoria que guarda a posicao do asteroide
				INC		M[SCORE]
				PUSH	FFFFh
				POP		M[LEDS]
				CALL	Display
Retorna2:		RET		


ComparaB:		MOV		R5,	P_BURACO
				MOV		R4,	M[POS_TIRO]
				CMP		M[R5],	R4
				BR.Z	EngoleTiro
Incrementa4:	INC		R5
				CMP		R5,	8048h				;Verifica se ja atingiu o ultimo endereco do vetor
				BR.Z	Retorna4
				CMP		M[R5],	R4
				BR.NZ	Incrementa4
EngoleTiro:		CALL	ApagaTiro
				PUSH	1851h
				POP		M[POS_TIRO]
Retorna4:		RET		
				
				



;----------------------------Criacao e Atualizacao dos Obstaculos-----------------------
;	Rotinas que criam e atualizam os obstaculos

AtualizaAst:	PUSH	0002h
				POP		M[ATUALIZA_AST]			;Reposicao da flag
				MOV		R5,	P_ASTEROIDE
				CMP		M[R5],	R0
				BR.NZ	MoveAsts
Incrementa2:	INC		R5
				CMP		R5,	8042h				;Verifica se ja atingiu o ultimo endereco do vetor
				BR.Z	AtualizaBur
				CMP		M[R5], R0
				BR.Z	Incrementa2
MoveAsts:		CALL 	ApagaAst
				MOV		R4,	M[R5]
				DEC		R4
				MOV		M[R5],	R4
				CALL	ReDesAst
				MOV		R6, R0
				MVBL	R6,	M[R5]
				CMP		R6,	R0
				BR.NZ	Incrementa2
				CALL	ApagaAst
				MOV		M[R5],	R0
				BR		Incrementa2

AtualizaBur:	MOV		R5,	P_BURACO
				CMP		M[R5],	R0
				BR.NZ	MoveBurs
IncrementaB2:	INC		R5
				CMP		R5,	8047h				;Verifica se ja atingiu o ultimo endereco do vetor
				BR.Z	Retorna
				CMP		M[R5], R0
				BR.Z	IncrementaB2
MoveBurs:		CALL 	ApagaBur
				MOV		R4,	M[R5]
				DEC		R4
				MOV		M[R5],	R4
				CALL	ReDesBur
				MOV		R6, R0
				MVBL	R6,	M[R5]
				CMP		R6,	R0
				BR.NZ	IncrementaB2
				CALL	ApagaBur
				MOV		M[R5],	R0
				BR		IncrementaB2
Retorna:		RET



CriacaoAst:		PUSH	000Ah					;Reposicao flag
				POP		M[CRIA_AST]
				CMP		M[VEZ_BURACO], R0		;Verifica se e a vez de criar um buraco ou um asteroide
				BR.Z	PartidaBuraco
				MOV		R3,	P_ASTEROIDE
Partida:		CMP		M[R3],	R0
				BR.Z	CriaAst
Incrementa:		INC		R3
				CMP		R3,	8042h
				BR.Z	Volta
				CMP		M[R3], R0
				BR.NZ	Incrementa
CriaAst:		CALL 	Random
				MOV		R1,	M[LINHA_AST]
				MOV		M[R3],	R1
				CALL	DesAst
Retorna3:		DEC		M[VEZ_BURACO]
				RET
Volta:			MOV		R3,	8031h
				BR		Partida
				
				
PartidaBuraco:	PUSH	0003h					
				POP		M[VEZ_BURACO]			;Repoe a flag que conta o numero de asteroides consecutivos que foram criados
				MOV		R3,	P_BURACO
PartidaB:		CMP		M[R3],	R0
				BR.Z	CriaBuraco
IncrementaB:	INC		R3
				CMP		R3,	8047h
				BR.Z	VoltaB
				CMP		M[R3],	R0
				BR.NZ	IncrementaB
CriaBuraco:		CALL	Random
				MOV		R1,	M[LINHA_AST]
				MOV		M[R3],	R1
				CALL	DesBuraco
				RET
VoltaB:			MOV		R3,	8042h
				BR		PartidaB
				
				
			
			
;----------------------------Geracao Numero Aleatorio---------------------------------
;	Rotina que gera um numero pseudoaleatorio para a linha onde serao gerados os
;		novos obstaculos
;			Entradas:	M[NINI]
;			Saidas:		M[NINI]


Random:			MOV		R1,	M[NINI]			
				MOV		R2,	M[NINI]
				SHR		R1, 1					;Testa o bit menos significativo do Ni
				BR.NC	SemExor					;Se for 1, ou seja, se houver carry, executara o XOR
				XOR		R2, MASCARA_RAND
SemExor:		ROR		R2, 1
				MOV		M[NINI], R2
				MOV		R1,	17h					
				DIV		R2,	R1					;Divisao por 17h para garantir que o numero esta entre 0 e 23 (decimal), que e o numero de linhas disponiveis
				CMP		R1,	R0					;Testa se o numero nao e 0 (zero), pois nao pode ser printado na primeira linha, onde estao desenhados os limites
				BR.Z	Random
				SHL		R1,	8					;Coloca o numero aleatorio entre 0 e 23 nos bits mais significativos (bits correspondentes a linha)
				MOV		R2, 4Fh					;O numero sera inserido na ultima coluna
				ADD		R1,	R2
				MOV		M[LINHA_AST], R1
				RET




;----------------------------Janela Inicial-------------------------------------------
;	Rotina que efetua a escrita de uma cadeia de caracteres no ecrã

EscString:		MOV		R2,	0015h
				MOV		R4,	FIM_TEXTO
Ciclo2:			PUSH	R2
				POP		M[IO_CURSOR]
				CMP     M[R1], R4
                BR.Z    FimEsc
				PUSH	M[R1]
                POP		M[IO_WRITE]
                INC     R1
				INC		R2
                BR      Ciclo2
FimEsc:         CMP		R5,	1d
				BR.NZ	FimEsc
				RET
	
	
	
	
;----------------------------Limites da janela----------------------------------------
;	Rotina que escreve os limites superior e inferior da janela ('#')
;			Entradas:	R2
;			Saidas:		---

EscLimite:		MOV 	M[IO_CURSOR],	R2		;Move o cursor para a primeira linha / ultima linha
				PUSH	HASH
				POP		M[IO_WRITE]
				INC		R2
				DEC		R1
				CMP		R1, R0
				BR.NZ	EscLimite
				RET
				
				
				
				
;----------------------------Desenho da nave e tiro e asteroides-----------------------
;	Rotina que escreve a nave na sua nova posicao apos ter sido acionada uma interrupcao
;	que gere movimento da mesma.

DesNave:		PUSH	M[POS_CANHAO]		
				POP		M[IO_CURSOR]
				PUSH	CANHAO
				POP		M[IO_WRITE]
				PUSH	M[POS_TOPO_NAVE]
				POP		M[IO_CURSOR]
				PUSH	TOPO_NAVE
				POP		M[IO_WRITE]
				PUSH	M[POS_CORPO_NAVE]
				POP		M[IO_CURSOR]
				PUSH	CORPO_NAVE
				POP		M[IO_WRITE]
				PUSH	M[POS_BAIXO_NAVE]
				POP		M[IO_CURSOR]
				PUSH	BAIXO_NAVE
				POP		M[IO_WRITE]	
				RET

DesTiro:		PUSH	M[POS_TIRO]
				POP		M[IO_CURSOR]
				PUSH	TIRO
				POP		M[IO_WRITE]
				RET
				
DesAst:			PUSH	M[LINHA_AST]
				POP		M[IO_CURSOR]
				PUSH	ASTEROIDE
				POP		M[IO_WRITE]
				RET
				
ReDesAst:		PUSH	M[R5]
				POP		M[IO_CURSOR]
				PUSH	ASTEROIDE
				POP		M[IO_WRITE]
				RET
				
DesBuraco:		PUSH	M[LINHA_AST]
				POP		M[IO_CURSOR]
				PUSH	BURACO_NEG
				POP		M[IO_WRITE]
				RET
				
ReDesBur:		PUSH	M[R5]
				POP		M[IO_CURSOR]
				PUSH	BURACO_NEG
				POP		M[IO_WRITE]
				RET
				
				
				
				
;----------------------------Apaga nave e tiro-----------------------------------------
;	Rotina que apaga a nave, escrevendo um caracter vazio (' ') na posicao onde estavam
;	os seus elementos.

ApagaNave:		PUSH	M[POS_CANHAO]			
				POP		M[IO_CURSOR]
				PUSH	LIMPA
				POP		M[IO_WRITE]
				PUSH	M[POS_TOPO_NAVE]
				POP		M[IO_CURSOR]
				PUSH	LIMPA
				POP		M[IO_WRITE]
				PUSH	M[POS_CORPO_NAVE]
				POP		M[IO_CURSOR]
				PUSH	LIMPA
				POP		M[IO_WRITE]
				PUSH	M[POS_BAIXO_NAVE]
				POP		M[IO_CURSOR]
				PUSH	LIMPA
				POP		M[IO_WRITE]	
				RET
				
ApagaTiro:		PUSH	M[POS_TIRO]
				POP		M[IO_CURSOR]
				PUSH	LIMPA
				POP		M[IO_WRITE]
				RET
				
ApagaAst:		PUSH	M[R5]
				POP		M[IO_CURSOR]
				PUSH	LIMPA
				POP		M[IO_WRITE]
				RET

ApagaBur:		PUSH	M[R5]
				POP		M[IO_CURSOR]
				PUSH	LIMPA
				POP		M[IO_WRITE]
				RET
				
				
				
				
;----------------------------Movimento da nave-----------------------------------------
;	Rotina que calcula a posicao que a nave ira ocupar apos ser acionada uma interrupcao
;	que gere movimento.
;			Entradas:	R5
;			Saidas:		---

Movimento:		ADD		M[POS_CANHAO],	R5
				ADD		M[POS_CORPO_NAVE],	R5
				ADD		M[POS_TOPO_NAVE],	R5
				ADD		M[POS_BAIXO_NAVE],	R5
				RET
				
				
				
				
;---------------------------Teste Colisão com Obstaculos------------------------------
;	Rotinas que testam colisoes entre a nave e os obstaculos
;	Se colidirem, o jogo acabara e sera chamada a rotina "PrintFimJogo"

VerificaMorte:	NOP
VerificaObs:	MOV		R5,	P_ASTEROIDE
				MOV		R4,	M[POS_CANHAO]
				CMP		R4,	M[R5]					;Compara a posicao do canhao com as posicoes dos asteroides
				JMP.Z	GameOver
				MOV		R4,	M[POS_TOPO_NAVE]
				CMP		R4,	M[R5]
				JMP.Z	GameOver
				MOV		R4,	M[POS_BAIXO_NAVE]
				CMP		R4,	M[R5]
				JMP.Z	GameOver
				MOV		R4,	M[POS_CORPO_NAVE]
				CMP		R4,	M[R5]
				JMP.Z	GameOver
IncrementaObs:	INC		R5
				CMP		R5,	8047h					;Verifica se chegou a ultima posicao do vetor
				JMP.Z	RetornaObs
				MOV		R4,	M[POS_CANHAO]
				CMP		R4,	M[R5]
				JMP.Z	GameOver
				MOV		R4,	M[POS_TOPO_NAVE]
				CMP		R4,	M[R5]
				JMP.Z	GameOver
				MOV		R4,	M[POS_BAIXO_NAVE]
				CMP		R4,	M[R5]
				JMP.Z	GameOver
				MOV		R4,	M[POS_CORPO_NAVE]
				CMP		R4,	M[R5]
				JMP.Z	GameOver
				JMP		IncrementaObs
RetornaObs:		RET		

				
				
				
				
;---------------------------Escrita no LCD--------------------------------------------
;		Rotina que escreve no LCD a posicao (linha e coluna) que a nave ocupa

EscreveLCD:		MOV		R4,	FIM_TEXTO
				MOV		R2,	M[POS_CANHAO]
				MVBH	R3, R2
				SHR		R3,	8
				AND		R2,	00FFh
				MOV		R1,	8000h
				MOV		M[CONTROLO_LCD],	R1
				PUSH	LINHA
				POP		R5
CicloLin:		PUSH	R1
				POP		M[CONTROLO_LCD]
				CMP		M[R5], R4
				BR.Z	SegueLCD
				PUSH	M[R5]
				POP		M[ESCRITA_LCD]
				INC		R5
				INC		R1
				BR		CicloLin
SegueLCD:		PUSH	R1
				POP		M[CONTROLO_LCD]
				MOV		R6,	000Ah
				DIV		R3,	R6
				ADD		R3,	0030h
				PUSH	R3
				POP		M[ESCRITA_LCD]
				INC		R1
				PUSH	R1
				POP		M[CONTROLO_LCD]
				ADD		R6,	0030h
				PUSH	R6
				POP		M[ESCRITA_LCD]
				ADD		R1,	0002h
				PUSH	COLUNA
				POP		R5
CicloCol:		PUSH	R1
				POP		M[CONTROLO_LCD]
				CMP		M[R5], R4
				BR.Z	SegueLCD2
				PUSH	M[R5]
				POP		M[ESCRITA_LCD]
				INC		R5
				INC		R1
				BR		CicloCol
SegueLCD2:		PUSH	R1
				POP		M[CONTROLO_LCD]
				MOV		R6,	000Ah
				DIV		R2,	R6
				ADD		R2,	0030h
				PUSH	R2
				POP		M[ESCRITA_LCD]
				INC		R1
				PUSH	R1
				POP		M[CONTROLO_LCD]
				ADD		R6,	0030h
				PUSH	R6
				POP		M[ESCRITA_LCD]
				RET
				
				
				
				
;----------------------------Escrita no Display----------------------------------------
;		Rotina que escreve no display de 7 segmentos o score atual da altura

Display: 		MOV 	R4, M[SCORE]
				MOV		R2,	000Ah
				DIV		R4,	R2
				ADD		R2,	30h
				PUSH	R2
				POP		M[DISPLAY1]
				MOV		R2,	000Ah
				DIV		R4,	R2
				ADD		R2,	30h
				PUSH	R2
				POP		M[DISPLAY2]
				MOV		R2,	000Ah
				DIV		R4,	R2
				ADD		R2,	30h
				PUSH	R2
				POP		M[DISPLAY3]
				MOV		R2,	000Ah
				DIV		R4,	R2
				ADD		R2,	30h
				PUSH	R2
				POP		M[DISPLAY4]
				RET							
				
				

				
;---------------------------Ciclo Infinito: Game Over---------------------------------
;	Rotinas que escrevem no ecra "Fim Jogo" e por baixo a pontuacao atingida
				
GameOver:		MOV		R1,	R0						
LimpaProx:		PUSH	R1							;Limpa a janela
				POP		M[IO_CURSOR]
				PUSH	LIMPA
				POP		M[IO_WRITE]
				CMP		R1,	1750h
				JMP.Z	PrintFimJogo
				INC		R1
				BR		LimpaProx

LimpaReset:		POP		R0
				MOV		R1,	R0
LimpaResProx:	PUSH	R1
				POP		M[IO_CURSOR]
				PUSH	LIMPA
				POP		M[IO_WRITE]
				INC		R1
				CMP		R1,	1751h
				BR.NZ	LimpaResProx
				RET
							
PrintFimJogo:	MOV		R1,	MSG_FIM_JOGO
				MOV		R2,	0C23h
				MOV		R4,	FIM_TEXTO
				MOV		R5, R0
				
CicloFimJogo:	PUSH	R2
				POP		M[IO_CURSOR]
				CMP     M[R1], R4
                BR.Z    LimpaLCD
				PUSH	M[R1]
                POP		M[IO_WRITE]
                INC     R1
				INC		R2
                BR      CicloFimJogo

LimpaLCD:		MOV		R1,	8000h
CicloLimpa:		MOV		M[CONTROLO_LCD],	R1
				CMP		R1,	800Fh
				BR.Z	PrintPont
				PUSH	LIMPA
				POP		M[ESCRITA_LCD]
				INC		R1
				BR		CicloLimpa				

PrintPont:		MOV		R1,	MSG_PONTUACAO
				MOV		R2,	0E20h
				MOV		R4,	FIM_TEXTO
				MOV		R5, R0
CicloPont:		PUSH	R2
				POP		M[IO_CURSOR]
				CMP     M[R1], R4
                BR.Z    PrintReset
				PUSH	M[R1]
                POP		M[IO_WRITE]
                INC     R1
				INC		R2
                BR      CicloPont
				
PrintReset:		MOV		R1,	MSG_RESET
				MOV		R2,	1014h
				MOV		R4,	FIM_TEXTO
				MOV		R5, R0
CicloReset1:	PUSH	R2
				POP		M[IO_CURSOR]
				CMP     M[R1], R4
                BR.Z    PrintScore
				PUSH	M[R1]
                POP		M[IO_WRITE]
                INC     R1
				INC		R2
                BR      CicloReset1
				
PrintScore:		MOV		R4,	0E30h
CicloScore:		PUSH	R4
				POP		M[IO_CURSOR]
				MOV		R2,	000Ah
				DIV		M[SCORE],	R2
				ADD		R2,	30h
				PUSH	R2
				POP		M[IO_WRITE]
				DEC		R4
				CMP		M[SCORE],	R0
				BR.NZ	CicloScore
				PUSH	1h
				POP		M[RESET]
				BR		CicloFinal
				
CicloFinal:		CMP		M[RESET],	R0
				BR.NZ	CicloFinal
				BR		Reset
				
Reset:			PUSH	0503h						;Reposicao de todas as flags e words para o seu valor inicial
				POP		M[POS_CANHAO]
				PUSH	0502h
				POP		M[POS_CORPO_NAVE]
				PUSH	0402h
				POP		M[POS_TOPO_NAVE]
				PUSH	0602h
				POP		M[POS_BAIXO_NAVE]
				PUSH	0000h
				POP		M[MOVE_CIMA]
				PUSH	0000h
				POP		M[MOVE_DIREITA]
				PUSH 	0000h
				POP 	M[MOVE_ESQUERDA]
				PUSH 	0000h
				POP 	M[MOVE_BAIXO]
				PUSH 	0000h
				POP 	M[DEU_TIRO]
				PUSH	0000h
				POP 	M[ATUALIZA_TIRO]
				MOV		R1,	8031h
CicloReset:		PUSH	0000h						;Limpeza dos vetores com as posicoes dos obstaculos (0000h)
				POP		M[R1]
				INC		R1
				CMP		R1,	8047h
				BR.NZ	CicloReset
				PUSH	000Ah
				POP 	M[CRIA_AST]
				PUSH	000Ch
				POP 	M[ATUALIZA_AST]
				PUSH	0000h
				POP 	M[LINHA_AST]
				PUSH	0003h
				POP 	M[VEZ_BURACO]
				PUSH	0000h
				POP 	M[SCORE]
				PUSH	0001h
				POP 	M[GAME_OVER]
				PUSH	0001h
				POP 	M[RESET]
				CALL	LimpaResProx
				PUSH 	0000h
				POP 	M[DISPLAY1]
				PUSH 	0000h
				POP 	M[DISPLAY2]
				PUSH 	0000h
				POP 	M[DISPLAY3]
				PUSH 	0000h
				POP 	M[DISPLAY4]
				JMP		RetomaReset
				
				
				
				
;----------------------------Programa principal----------------------------------------
							
Inicio:			MOV		R7, SP_INICIAL
				MOV		SP,	R7
				PUSH	INT_MASK
				POP		M[INT_MASK_ADDR]		;Permite as interrupcoes I0, I1, I2, I3, I4, I5 e I15
				ENI
				PUSH	FFFFh
				POP		M[IO_CURSOR]			;Inicializacao cursor
				MOV		R1,	Mensagem
				CALL	EscString
RetomaReset:	MOV 	R2, 0000h				;Endereco de retorno apos ser efetuado o reset do jogo
				MOV		R1, 50h
				CALL	EscLimite
				MOV		R2,	1700h
				MOV 	R1,	50h
				CALL	EscLimite
				CALL	DesNave
				PUSH	ENABLE_TIMER
				POP		M[IO_TIME_ON]
				PUSH	1h
				POP		M[IO_TIME]
				MOV		R7,	1h
				MOV		M[SCORE],	R0
Ciclo:			CMP		M[MOVE_CIMA], R0
				CALL.NZ	MoveCima
				CMP		M[MOVE_DIREITA], R0
				CALL.NZ	MoveDireita
				CMP		M[MOVE_ESQUERDA], R0
				CALL.NZ	MoveEsquerda
				CMP		M[MOVE_BAIXO], R0
				CALL.NZ	MoveBaixo
				CMP		M[DEU_TIRO], R0
				CALL.NZ	DeuTiro
				CMP		M[ATUALIZA_TIRO], R7
				CALL.Z	AtualizaTiro
				CMP		M[CRIA_AST], R0
				CALL.Z	CriacaoAst
				CMP		M[ATUALIZA_AST], R0
				CALL.Z	AtualizaAst
				CALL	VerificaMorte
				JMP		Ciclo
