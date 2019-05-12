#   Projeto 2 de Inteligencia Artificial
#
#   Grupo 22
#
#   Miguel Sena - 86420
#   Joao Palet  - 86447 

import itertools
import copy

class Node():
    def __init__(self, prob, parents=[]):
        self.parents = parents
        self.prob = prob
    
    def computeProb(self, evid):
        probs = self.prob
        for p in self.parents:
            if evid[p]==0:
                probs = probs[0]
            else:
                probs = probs[1]

        return [1-probs, probs]


class BN():
    def __init__(self, gra, prob):
        self.gra = gra
        self.prob = prob

    def computePostProb(self, evid):
        s = 0
        t = 0
        evid = list(evid)
        evid2 = copy.deepcopy(evid)
    
        for i in range(0, len(evid)):
            if (evid[i] == -1):
                evid[i] = 1
                evid2[i] = []

        ev = self.permut(evid)
        for e in ev:
            s = s + self.computeJointProb(e)

        ev = self.permut(evid2)
        for e in ev:
            t = t + self.computeJointProb(e)

        return s/t

    def permut(self, evid):
        empty = []
        ev = []
        s = 0
        rep = 0

        for e in evid:
            if (e==[]):
                rep = rep+1	
                empty.append(s)
            s = s + 1

        l = list(itertools.product([0, 1], repeat=rep))

        aux = list(evid)
        for k in range(0, len(l)):
            for i in range(0, len(empty)):
                aux[empty[i]] = l[k][i]
            ev.append(aux)
            aux = list(evid)

        return ev

		        
    def computeJointProb(self, evid):
        res = 1
        i = 0
        for node in self.prob:
            res = res*node.computeProb(evid)[evid[i]] 
            i = i+1

        return res