#_____________Disciplina: Fundamentos de Programacao_____________#
#__________________Joao Lopes Palet de Almeida___________________#
#_____________________________86447______________________________#




#1.1 - VERSAO SIMPLIFICADA




#1.1.1

def gera_chave1(letras):
    #Funcao que recebe um argumento, letras, que consiste num tuplo de 25 caracteres, e devolve um tuplo de 5 tuplos de caracteres, cada um com 5 elementos, cujos elementos sao os caracteres de letras.
    i2, x, y, tup, chave = 0, 0, 5, (), ()
    for i1 in range(0, 5):
        tup = ()
        for i2 in range(x, y):
            tup = tup + (letras[i2],)
        x, y = x + 5, y + 5
        chave = chave + (tup,)
    return chave




#1.1.2

def obtem_codigo1(car, chave):
    #Funcao que recebe dois argumentos, car, consistindo num caractere, e chave, consistindo numa chave, e devolve uma cadeia de 2 caracteres, correspondendo ao codigo do caractere de acordo com a chave.
    res = ''
    for el in car:
        for i in range(len(chave)):
            for i1 in range(len(chave[i])):
                if chave[i][i1] == el: #Encontra o indice do lugar que o caractere ocupa dentro do tuplo a que pertence em chave.
                    res = res + str(i) + str(i1)
    return res

def codifica1(cad, chave):
    #Funcao que recebe dois argumentos, cad, uma cadeia de caracteres, e chave, uma chave, e devolve a cadeia de caracteres correspondente a encriptacao de cad. 
    return obtem_codigo1(cad, chave)  




#1.1.3

def obtem_car1(cod, chave):
    #Funcao que recebe dois argumentos, cod, consistindo numa cadeia de 2 digitos, e chave, consistindo numa chave, e devolve o caractere correspondente ao codigo cod.
    res, i = '', 0
    while i <= (len(cod) - 1):
        res = res + chave[int(cod[i])][int(cod[i+1])]
        i = i + 2
    return res

def descodifica1(cad_codificada, chave):
    #Funcao que recebe dois argumentos, cad_codificada, uma cadeia de caracteres encriptada, e chave, a chave usada na encriptacao, e devolve uma cadeia de caracteres correspondente a mensagem original.
    res = obtem_car1(cad_codificada, chave)
    return res








#1.2 - VERSAO FINAL




#1.2.1

def gera_chave2(letras):
    #A funcao recebe um argumento, letras, consistindo num tuplo de caracteres e devolve um tuplo de tuplos, cujos elementos sao os de letras.
    num_tuplos, el_tuplo, res = 0, 0, ()
    while num_tuplos**2 < len(letras):
        num_tuplos = num_tuplos + 1
    while el_tuplo*num_tuplos < len(letras):
        el_tuplo = el_tuplo + 1
    def preenche(letras):
        if letras == ():
            return ()
        else:
            return (letras[0:el_tuplo],) + preenche(letras[el_tuplo:])
    return preenche(letras)




#1.2.2

def obtem_codigo2(car, chave):
    #Funcao que recebe dois argumentos, car, consistindo num caractere, e chave, consistindo numa chave e devolve uma cadeia de 2 caracteres, correspondendo ao codigo do caractere de acordo com chave. Se o caractere nao pertencer a chave, a funcao deve devolver 'XX'.
    pertence = False
    for i1 in range(len(chave)):
        for i2 in range(len(chave[i1])):
            if chave[i1][i2] == car:
                pertence = True
                res = str(i1) + str(i2)
    if pertence == False:
        res = 'XX'
    return res

def codifica2(cad, chave):
    #Funcao que recebe dois argumentos, cad, uma cadeia de caracteres, e chave, uma chave. A funcao utiliza a funcao obtem_codigo2 e devolve uma cadeia de caracteres, correspondendo a encriptacao de cad.
    res = ''
    for el in cad:
        res = res + obtem_codigo2(el, chave)
    return res




#1.2.3

def obtem_car2(cod, chave):
    #Funcao que recebe dois argumentos, cod, consistindo numa cadeia de 2 digitos ou 'XX', e chave, consistindo numa chave. A funcao devolve o caractere correspondente ao codigo cod, ou '?' se cod for 'XX'.    
    res, i = '', 0
    while i <= (len(cod) - 1):
        if cod[i] == 'X':
            res = res + '?'
        else:
            res = res + chave[int(cod[i])][int(cod[i+1])]
        i = i + 2
    return res

def descodifica2(cad_codificada, chave):
    #Funcao que recebe dois argumentos, cad_codificada, a encriptacao de uma cadeia de caracteres, e chave, a chave usada na encriptacao. A funcao devolve uma cadeia de caracteres, correspondendo a mensagem original.
    return obtem_car2(cad_codificada, chave)