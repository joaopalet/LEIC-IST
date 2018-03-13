%	Joao Almeida - 86447

:- include('SUDOKU').

%---------------------------------------------------------------------
%		   3.1. - Predicados para a propagacao de mudancas
%---------------------------------------------------------------------

%	3.1.1.

%	tira_num_aux(Num, Puz, Pos, N_Puz) :
%		N_puz e' o puzzle resultante de tirar o numero Num da posicao
%		Pos do puzzle Puz.
tira_num_aux(Num, Puz, Pos, N_Puz) :-
	puzzle_ref(Puz, Pos, Cont),
	subtract(Cont, [Num], N_Cont),
	puzzle_muda_propaga(Puz, Pos, N_Cont, N_Puz).

%	tira_num(Num, Puz, Posicoes, N_Puz) :
%		N_Puz e' o puzzle resultante de tirar o numero Num de todas as
%		posicoes em Posicoes do puzzle Puz.
tira_num(Num, Puz, Posicoes, N_Puz) :-
	percorre_muda_Puz(Puz, tira_num_aux(Num), Posicoes, N_Puz).

%	3.1.2.

%	tira_num(Num, Puz, Posicoes, N_Puz) :
%		N_Puz e' o puzzle resultante de tirar o numero Num de todas as
%		posicoes em Posicoes do puzzle Puz.
puzzle_muda_propaga(Puz, Pos, Cont, Puz) :-
	puzzle_ref(Puz, Pos, Cont),
	!.

puzzle_muda_propaga(Puz, Pos, Cont, N_Puz) :-
	length(Cont, 1),
	puzzle_muda(Puz, Pos, Cont, Puz_Aux),
	posicoes_relacionadas(Pos, Posicoes_Rel),
	nth1(1, Cont, Num),
	tira_num(Num, Puz_Aux, Posicoes_Rel, N_Puz),
	!.

puzzle_muda_propaga(Puz, Pos, Cont, N_Puz) :-
	puzzle_muda(Puz, Pos, Cont, N_Puz).

%---------------------------------------------------------------------
%	     3.2. - Predicados para a inicializacao de puzzles
%---------------------------------------------------------------------

%	3.2.1

%	possibilidades(Pos, Puz, Poss) :
%		Poss e' a lista de numeros possiveis para a posicao Pos do
%		puzzle Puz. (Apenas deve ser usado para posicoes cujo conteudo
%		nao e' uma sequencia unitaria.)
possibilidades(Pos, Puz, Cont) :-
	puzzle_ref(Puz, Pos, Cont),
	length(Cont, 1),
	!.

possibilidades(Pos, Puz, Poss) :-
	numeros(L),
	posicoes_relacionadas(Pos, Posicoes),
	conteudos_posicoes(Puz, Posicoes, Conteudos),	
	encontra_unitarias(Conteudos, [], Cont_Unitarios),	%	Apenas as posicoes com conteudo unitaras influenciam as possibilidades
	subtract(L, Cont_Unitarios, Poss).

%	encontra_unitarias(Conteudos, L_Aux, List) :
%		List e' a lista com os conteudos das listas unitarias da lista 
%		de listas Conteudos.
encontra_unitarias([P|R], L_Aux, List) :-
	length(P, 1),
	append(P, L_Aux, L_Aux2),
	encontra_unitarias(R, L_Aux2, List).

encontra_unitarias([_|R], L_Aux, List) :-
	encontra_unitarias(R, L_Aux, List).

encontra_unitarias([], List, List).

%	3.2.2.

%	inicializa_aux(Puz, Pos, N_Puz) :
%		N_Puz e' o puzzle resultante de colocar na posicao Pos
%		do puzzle Puz a lista com os numeros possiveis para essa posicao.
%		(Se o conteudo da posicao Pos de Puz ja for uma lista unitaria, 
%		nada e' alterado.)
inicializa_aux(Puz, Pos, N_Puz) :-
	possibilidades(Pos, Puz, Poss),
	puzzle_muda_propaga(Puz, Pos, Poss, N_Puz).

%	inicializa(Puz, N_Puz) :
%		N_Puz e' o puzzle resultante de inicializar o puzzle Puz.
inicializa(Puz, N_Puz) :-
	todas_posicoes(Todas_Posicoes),
	percorre_muda_Puz(Puz, inicializa_aux, Todas_Posicoes, N_Puz),
	!.

%---------------------------------------------------------------------
%	        3.3. - Predicados para a inspecao de puzzles
%---------------------------------------------------------------------

%	3.3.1

%	so_aparece_uma_vez(Puz, Num, Posicoes, Pos_Num) :
% 		Significa que Num so aparece numa das posicoes da lista Posicoes
%		do puzzle Puz, e que essa posicao e' Pos_Num.
so_aparece_uma_vez(Puz, Num, Posicoes, Pos_Num) :-
	conteudos_posicoes(Puz, Posicoes, Conteudos),
	conta_ocorrencias(Conteudos, Num, 0, Num_Vezes),
	Num_Vezes =:= 1,
	encontra_posicao(Puz, Posicoes, Num, Pos_Num),
	!.

%	encontra_posicao(Puz, Lista_Posicoes, Num, Pos) :
%		Sabendo que Num ocorre numa das posicoes de Lista_Posicoes, Pos 
%		sera essa posicao.
encontra_posicao(_, [], _, _).

encontra_posicao(Puz, [P|_], Num, P) :-
	puzzle_ref(Puz, P, Cont),
	member(Num, Cont).

encontra_posicao(Puz, [_|R], Num, Pos) :-
	encontra_posicao(Puz, R, Num, Pos).

%	conta_ocorrencias(Lista_De_Listas, Num, Acum, Contador) :
%		Contador tera o valor do numero de vezes que Num ocorre em 
%		Lista_De_Listas.
conta_ocorrencias([], _, Contador, Contador).

conta_ocorrencias([P|R], Num, Acum, Contador) :-
	member(Num, P),
	Acum2 is Acum+1,
	conta_ocorrencias(R, Num, Acum2, Contador),
	!.

conta_ocorrencias([_|R], Num, Acum, Contador) :-
	conta_ocorrencias(R, Num, Acum, Contador).

%	3.3.2

%	inspecciona_num(Posicoes, Puz, Num, N_Puz) :
%		N_Puz e' o resultado de inspeccionar o grupo cujas posicoes 
%		sao Posicoes, para o numero Num.
inspecciona_num(Posicoes, Puz, Num, N_Puz) :-
	so_aparece_uma_vez(Puz, Num, Posicoes, Pos_Num),
	puzzle_ref(Puz, Pos_Num, Cont),
	length(Cont, Len),
	Len =\= 1,
	puzzle_muda_propaga(Puz, Pos_Num, [Num], N_Puz),
	!.	%	Nao se pretende que apresente mais solucoes iguais

inspecciona_num(_, N_Puz, _, N_Puz).

%	inspecciona_grupo(Puz, Gr, N_Puz) :
%		inspecciona o grupo cujas posicoes sao as de Gr, do puzzle
%		Puz para cada um dos numeros possiveis, sendo o resultado o
%		puzzle N_Puz.
inspecciona_grupo(Puz, Gr, N_Puz) :-
	numeros(L),
	percorre_muda_Puz(Puz, inspecciona_num(Gr), L, N_Puz),
	!.

%	inspecciona(Puz, N_Puz) :
%		Inspecciona cada um dos grupos do puzzle Puz, para cada um dos
%		numeros possiveis, sendo o resultado o puzzle N_Puz.
inspecciona(Puz, N_Puz) :-
	grupos(Gr),
	percorre_muda_Puz(Puz, inspecciona_grupo, Gr, N_Puz).

%---------------------------------------------------------------------
%	      3.4. - Predicados para a verificacao de solucoes
%---------------------------------------------------------------------

%	grupo_correcto(Puz, Nums, Gr) :
%		O grupo do puzzle Puz cujas posicoes sao as de Gr esta correto,
%		ou seja, contem todos os numeros da lista Nums, sem repeticoes.
grupo_correcto(Puz, Nums, Gr) :-
	conteudos_posicoes(Puz, Gr, Conteudos),
	grupo_correcto_aux(Nums, Conteudos, Lista),
	Lista == [].	%	Lista diferenca entre Nums e os Conteudos unitarios das posicoes do grupo

%	grupo_correcto_aux(Nums, Conteudos, Lista) :
%		Lista sera a lista com os numeros de Nums que nao ocorrem
%		nas posicoes de Conteudos, ou que ocorrem mais do que uma
%		vez. O grupo estara correto se a Lista for vazia, ou seja, 
%		todos os numeros de Nums ocorrerem uma vez no grupo.
grupo_correcto_aux(Lista, [], Lista).

grupo_correcto_aux(Nums, [P|R], Lista) :-
	length(P, 1),
	subtract(Nums, P, Lista_Aux),
	grupo_correcto_aux(Lista_Aux, R, Lista),
	!.

grupo_correcto_aux(Nums, [_|R], Lista) :-
	grupo_correcto_aux(Nums, R, Lista).

%	solucao(Puz) :
%		Puzzle Puz e' uma solucao.
solucao(Puz) :-
	numeros(L),
	grupos(Gr),
	solucao_aux(Puz, L, Gr).

%	solucao-aux(Puz, L, Gr) :
%		Testa se todos os grupos Gr, para os numeros de 1 a L, estao
%		corretos no puzzle Puz.
solucao_aux(_, _, []) :-
	!.

solucao_aux(Puz, L, [P|R]) :-
	grupo_correcto(Puz, L, P),
	solucao_aux(Puz, L, R).

%---------------------------------------------------------------------
%	                3.5. - Predicado resolve/2
%---------------------------------------------------------------------

%	resolve(Puz, Sol) :
%		Puzzle Sol e' um(a) solucao do puzzle Puz
resolve(Puz, Sol) :-
	inicializa(Puz, Puz_Inic),
	inspecciona(Puz_Inic, Puz_Inspec),
	todas_posicoes(Posicoes),
	resolve_aux(Puz_Inspec, Posicoes, Sol),
	!.	%	Apenas apresenta uma solucao

%	resolve_aux(Puz, Posicoes, Sol) :
%		Sol e' (um)a solucao do puzzle Puz. Posicoes e' uma lista com
%		com todas as posicoes do puzzle.
resolve_aux(Sol, _, Sol) :-
	solucao(Sol).

resolve_aux(Puz, [P|R], Sol) :-
	puzzle_ref(Puz, P, Cont),
	length(Cont, Len),
	Len > 1,
	member(Num, Cont),
	puzzle_muda_propaga(Puz, P, [Num], N_Puz),
	resolve_aux(N_Puz, R, Sol).

resolve_aux(Puz, [P|R], Sol) :-
	puzzle_ref(Puz, P, Cont),
	length(Cont, 1),
	resolve_aux(Puz, R, Sol).

%-------------------------------------------------------------------------