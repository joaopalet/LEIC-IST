#___________________Fundamentos de Programacao___________________#
#__________________Joao Lopes Palet de Almeida___________________#
#_____________________________86447______________________________#


letras = ('A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z')




#2.1. - TIPO POSICAO


#Construtor

def numero(x):
    '''Funcao que testa se o argumento (x) e um numero'''
    return isinstance(x, int)


def faz_pos(l, c):
    '''Funcao que recebe dois argumentos, l e c, e devolve um argumento do tipo posicao, testando a validade dos argumentos'''
    if numero(l) and numero(c) and 0 <= l and 0 <= c:
        return (l, c)
    else:
        raise ValueError('faz_pos: argumentos errados')
    



#Seletores

def linha_pos(p):
    '''Funcao que recebe uma posicao e devolve a linha correspondente'''
    return p[0]
    

def coluna_pos(p):
    '''Funcao que recebe uma posicao e devolve a coluna correpondente'''
    return p[1]




#Reconhecedor

def e_pos(arg):
    '''Funcao que recebe um argumento e testa se e do tipo posicao'''
    return isinstance(arg, tuple) and numero(arg[0]) and numero(arg[1]) and 0<=arg[0] and 0<=arg[1]
    
    
    
    
#Teste

def pos_iguais(p1, p2):
    '''Funcao que recebe dois argumentos do tipo posicao e testa se sao iguais'''
    return p1[0] == p2[0] and p1[1] == p2[1]








#2.2. - TIPO CHAVE


#Construtores

def preenche_linha(arg):
    '''Funcao que recebe uma lista de 25 strings, e converte-a num lista composta por 5 listas cada uma com 5 elementos'''
    if arg == []:
        return []
    else:
        return [arg[0:5],] + preenche_linha(arg[5:])

def lista_mgc(mgc):
    '''Funcao que recebe uma string e devolve uma lista com os seus elementos, excepto os repetidos e os espacos'''
    list_mgc = []
    for el in mgc:
        if el not in list_mgc and el != ' ':
            list_mgc = list_mgc + [el]
    return list_mgc

def junta_letras(list_mgc, l):
    '''Funcao que recebe duas listas, e completa a primeira com os elementos da segunda que a primeira nao contem'''
    for el in l:
        if el not in list_mgc and el != ' ':
            list_mgc = list_mgc + [el]
    return list_mgc

def ordem_espiral(s, pos):
    '''Funcao que recebe uma string correspondente ao sentido da rotacao (s) e uma posicao de inicio (pos) e devolve a ordem pela qual os numeros de 1 a 15 irao aparecer numa espiral contruida desse modo'''
    if pos == (0, 0):
        if s == 'r':
            ordem = [1, 2, 3, 4, 5, 16, 17, 18, 19, 6, 15, 24, 25, 20, 7, 14, 23, 22, 21, 8, 13, 12, 11, 10, 9]
        elif s == 'c':
            ordem = [1, 16, 15, 14, 13, 2, 17, 24, 23, 12, 3, 18, 25, 22, 11, 4, 19, 20, 21, 10, 5, 6, 7, 8, 9]
    elif pos == (0, 4):
        if s == 'r':
            ordem = [13, 14, 15, 16, 1, 12, 23, 24, 17, 2, 11, 22, 25, 18, 3, 10, 21, 20, 19, 4, 9, 8, 7, 6, 5]
        elif s == 'c':
            ordem = [5, 4, 3, 2, 1, 6, 19, 18, 17, 16, 7, 20, 25, 24, 15, 8, 21, 22, 23, 14, 9, 10, 11, 12, 13]
    elif pos == (4, 4):
        if s == 'r':
            ordem = [9, 10, 11, 12, 13, 8, 21, 22, 23, 14, 7, 20, 25, 24, 15, 6, 19, 18, 17, 16, 5, 4, 3, 2, 1]
        elif s == 'c':
            ordem = [9, 8, 7, 6, 5, 10, 21, 20, 19, 4, 11, 22, 25, 18, 3, 12, 23, 24, 17, 2, 13, 14, 15, 16, 1]
    elif pos == (4, 0):
        if s == 'r':
            ordem = [5, 6, 7, 8, 9, 4, 19, 20, 21, 10, 3, 18, 25, 22, 11, 2, 17, 24, 23, 12, 1, 16, 15, 14, 13]
        elif s == 'c':
            ordem = [13, 12, 11, 10, 9, 14, 23, 22, 21, 8, 15, 24, 25, 20, 7, 16, 17, 18, 19, 6, 1, 2, 3, 4, 5]
    return ordem


def gera_chave_linhas(l, mgc):
    '''Funcao que recebe dois argumentos, l e mgc, correspondentes a um tuplo de 25 letras e a uma cadeia de caracteres, e devolve a chave gerada por eles usando uma disposicao por linhas'''
    diferentes = []
    list_mgc = junta_letras(lista_mgc(mgc), l)
    for e in l:
        if e not in diferentes:
            diferentes += [e,]
    if diferentes != list(l) or ' ' in diferentes:
        raise ValueError('gera_chave_linhas: argumentos errados')
    for el in list_mgc:
        if el not in letras:
            raise ValueError('gera_chave_linhas: argumentos errados')  
    return preenche_linha(list_mgc)    


def gera_chave_espiral(l, mgc, s, pos):
    '''Funcao que recebe quatro argumentos, l, correspondendo a a um tuplo de 25 letras, uma string (mgc) correspondendo a uma cadeia de caracteres, uma string que indica o sentido da geracao da matriz em espiral (s) e a posicao de origem (pos) e devolve a matriz gerada usando a disposicao em espiral de acordo com os argumentos recebidos'''
    list_mgc = junta_letras(lista_mgc(mgc), l)
    diferentes = []
    entradas_matriz = []
    for e in l:
        if e not in diferentes:
            diferentes += [e,]
    if diferentes != list(l) or ' ' in diferentes:
        raise ValueError('gera_chave_espiral: argumentos errados')    
    for el in list_mgc:
        if el not in letras:
            raise ValueError('gera_chave_espiral: argumentos errados')
    if s != 'r' and s != 'c':
        raise ValueError('gera_chave_espiral: argumentos errados')
    if pos != (0, 0) and pos != (4, 0) and pos != (0, 4) and pos != (4, 4):
        raise ValueError('gera_chave_espiral: argumentos errados')
    ordem = ordem_espiral(s, pos)
    for i in ordem:
        entradas_matriz += list_mgc[i-1]
    return preenche_linha(entradas_matriz)    
    
    
    
      
#Seletor
    
def ref_chave(c, p):
    '''Funcao que recebe dois argumentos, uma chave (c) e uma posicao (p)'''
    return c[linha_pos(p)][coluna_pos(p)]




#Modificador

def muda_chave(c, p, l):
    '''Funcao que recebe como argumentos uma chave (c), uma posicao (p) e a letra (l) e devolve a chave c com a letra l na posicao p'''
    c[linha_pos(p)][coluna_pos(p)] = l
    return c




#Reconhecedor

def e_chave(arg):
    '''Funcao que recebe um argumento e devolve True se for do tipo chave e False caso contrario'''
    diferentes = []
    for e in arg:
        for letra in e:
            if letra not in diferentes:
                diferentes += [letra,]
    if preenche_linha(diferentes) != (arg):
        return False
    if len(arg) != 5:
        return False
    else:
        for e in arg:
            if len(e) != 5:
                return False
            else:
                for letra in e:
                    if letra not in letras:
                        return False
    return True                    




#Transformador

def string_chave(c):
    '''Funcao que recebe uma chave (c) e devolve uma cadeia de caracteres que sendo impressa apresenta as letras de c dispostas numa matriz 5 por 5'''
    string = ''
    for el in c:
        for letra in el:
            string += letra + ' '
        string += '\n'
    return string








#4. - FUNCOES A DESENVOLVER


#4.1.

def digramas(mens):
    '''Funcao que recebe como argumento uma cadeia de caracteres correspondente a uma mensagem (mens) e devolve a cadeia de caracteres correspondente aos diagramas transformados sem espacos'''
    lst_mens = []
    for e in mens:
        if e != ' ':
            lst_mens += e
    def digramas_aux(lst_mens):        
        if lst_mens == []:
            return ''
        elif len(lst_mens) == 1:                    #se sobrar apenas um elemento, arescentar 'X'
            return str(lst_mens[0]) + 'X'
        elif lst_mens[0] != lst_mens[1]:            #se os caracteres forem diferentes, ambos se mantem
            return str(lst_mens[0]) + str(lst_mens[1]) + digramas_aux(lst_mens[2:])
        else:                                       #se os caracteres forem iguais, acrescentar um 'X' mantendo o segundo caractere de seguida
            return str(lst_mens[0]) + 'X' + digramas_aux(lst_mens[1:])
    return digramas_aux(lst_mens)




#4.2.

def encontra_pos(car, chave):
    '''Funcao que recebe dois argumentos, um caractere (car) e uma chave, e devolve a posicao correspodente na chave'''
    for l in range(len(chave)):             #l corresponde a uma linha da chave
        if car in chave[l]:
            linha = l
            for c in range(len(chave[l])):              #c corresponde a uma coluna da chave
                if car == chave[l][c]:
                    coluna = c
    return faz_pos(linha, coluna)


def figura(digrm, chave):
    '''Funcao que recebe dois argumentos, uma cadeia com dois caracteres (digrm) e uma chave, e devolve um tuplo de 3 elementos onde esta representada a figura que os caracteres fazem na matriz, e a posicao de cada um deles nesta'''
    pos1 = encontra_pos(digrm[0], chave)
    pos2 = encontra_pos(digrm[1], chave)
    if linha_pos(pos1) == linha_pos(pos2):
        fig = 'l'                               #estao na mesma linha
    elif coluna_pos(pos1) == coluna_pos(pos2):
        fig = 'c'                               #estao na mesma coluna
    else:
        fig = 'r'                               #sao cantos opostos de um retangulo, estao em linhas e colunas diferentes
    return (fig, pos1, pos2)




#4.3.

def encripta_pos_linha(pos):
    '''Funcao que recebe como argumento uma posicao e devolve a posicao codificada na linha'''
    if coluna_pos(pos) < 4:
        pos_encr = (linha_pos(pos), coluna_pos(pos) + 1)
    else:
        pos_encr = (linha_pos(pos), 0)
    return pos_encr

def desencripta_pos_linha(pos):
    '''Funcao que recebe como argumento uma posicao codificada e devolve a posicao descodificada na linha'''
    if coluna_pos(pos) > 0:
        pos_desencr = (linha_pos(pos), coluna_pos(pos) - 1)
    else:
        pos_desencr = (linha_pos(pos), 4)
    return pos_desencr    


def codifica_l(pos1, pos2, inc):
    '''Funcao que recebe tres argumentos, duas posicoes na mesma linha e um inteiro (inc) que toma o valor de 1 se se prentender fazer uma encriptacao e -1 se se pretender fazer uma desencriptacao. Devolve as posicoes correspondentes as posicoes encriptadas/desencriptadas'''
    if inc == 1:                        #encriptacao
        pos1_cod = encripta_pos_linha(pos1)
        pos2_cod = encripta_pos_linha(pos2)
    elif inc == -1:                     #desencriptacao
        pos1_cod = desencripta_pos_linha(pos1)
        pos2_cod = desencripta_pos_linha(pos2)        
    return (pos1_cod, pos2_cod)




#4.4.

def encripta_pos_coluna(pos):
    '''Funcao que recebe como argumento uma posicao e devolve a posicao codificada na coluna'''
    if linha_pos(pos) < 4:
        pos_encr = (linha_pos(pos) + 1, coluna_pos(pos))
    else:
        pos_encr = (0, coluna_pos(pos))
    return pos_encr

def desencripta_pos_coluna(pos):
    '''Funcao que recebe como argumento uma posicao codificada e devolve a posicao descodificada na coluna'''
    if linha_pos(pos) > 0:
        pos_desencr = (linha_pos(pos) -1, coluna_pos(pos))
    else:
        pos_desencr = (4, coluna_pos(pos))
    return pos_desencr    


def codifica_c(pos1, pos2, inc):
    '''Funcao que recebe tres argumentos, duas posicoes na mesma coluna e um inteiro (inc) que toma o valor de 1 se se prentender fazer uma encriptacao e -1 se se pretender fazer uma desencriptacao. Devolve as posicoes correspondentes as posicoes encriptadas/desencriptadas'''
    if inc == 1:                        #encriptacao
        pos1_cod = encripta_pos_coluna(pos1)
        pos2_cod = encripta_pos_coluna(pos2)
    elif inc == -1:                     #desencriptacao
        pos1_cod = desencripta_pos_coluna(pos1)
        pos2_cod = desencripta_pos_coluna(pos2)        
    return (pos1_cod, pos2_cod)




#4.5.

def codifica_r(pos1, pos2):
    '''Funcao que recebe dois argumentos, duas posicoes em linhas e colunas diferentes numa chave, e devolve um tuplo com as as duas posicoes correspondentes as letras encriptadas/desencriptadas'''
    pos1_cod = (linha_pos(pos1), coluna_pos(pos2))                      #mantem a cordenada respetiva a linha, e adota a coordenada da coluna do outro caractere
    pos2_cod = (linha_pos(pos2), coluna_pos(pos1))
    return (pos1_cod, pos2_cod)




#4.6.

def codifica_digrama(digrm, chave, inc):
    '''Funcao que recebe tres argumentos, um digrama (digrm), uma chave e o inteiro inc, que toma o valor de 1 se se prentender fazer uma encriptacao e -1 se se pretender fazer uma desencriptacao. Devolve o digrama correspondente a encriptacao/desencriptacao usando a chave'''
    pos_0 = encontra_pos(digrm[0], chave)
    pos_1 = encontra_pos(digrm[1], chave)
    if inc == 1:                                    #encriptacao
        if linha_pos(pos_0) == linha_pos(pos_1):
            return ref_chave(chave, encripta_pos_linha(pos_0)) + ref_chave(chave, encripta_pos_linha(pos_1))
        elif coluna_pos(pos_0) == coluna_pos(pos_1):
            return ref_chave(chave, encripta_pos_coluna(pos_0)) + ref_chave(chave, encripta_pos_coluna(pos_1))
        else:  
            return ref_chave(chave, codifica_r(pos_0, pos_1)[0]) + ref_chave(chave, codifica_r(pos_0, pos_1)[1])
    elif inc == -1:                                 #desencriptacao
        if linha_pos(pos_0) == linha_pos(pos_1):
            return ref_chave(chave, desencripta_pos_linha(pos_0)) + ref_chave(chave, desencripta_pos_linha(pos_1))
        elif coluna_pos(pos_0) == coluna_pos(pos_1):
            return ref_chave(chave, desencripta_pos_coluna(pos_0)) + ref_chave(chave, desencripta_pos_coluna(pos_1))
        else:  
            return ref_chave(chave, linha_pos(codifica_r(pos_0, pos_1))) + ref_chave(chave, coluna_pos(codifica_r(pos_0, pos_1)))
        



#4.7.

def codifica(mens, chave, inc):
    '''Funcao que recebe tres argumentos, uma mensagem (mens), uma chave e o inteiro inc, que toma o valor de 1 se se prentender fazer uma encriptacao e -1 se se pretender fazer uma desencriptacao. Devolve a mensagem correspondente a encriptacao/desencriptacao de mens usando a chave'''
    mens = digramas(mens)                       #transforma a mensagem em digramas transformados sem espacos
    def codifica_aux(mens, chave, inc):
        if mens == '':
            return ''
        else:
            return codifica_digrama(mens[:2], chave, inc) + codifica_aux(mens[2:], chave, inc)                  #codifica ou descodifica (de acordo com inc) digrama a digrama
    return codifica_aux(mens, chave, inc)