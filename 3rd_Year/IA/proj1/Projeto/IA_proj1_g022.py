#   Projeto de Inteligencia Artificial - Parte 1
#
#   Miguel Sena -  86420
#   Joao Palet  -  86447



#-------------------------------------------------------------------------------

import copy
from search import*



# TAI content
def c_peg ():
    return "O"
def c_empty ():
    return "_"
def c_blocked ():
    return "X"
def is_empty (e):
    return e == c_empty()
def is_peg (e):
    return e == c_peg()
def is_blocked (e):
    return e == c_blocked()



# TAI pos
# Tuplo (l, c)
def  make_pos (l, c):
    return (l, c)
def pos_l (pos):
    return pos[0]
def pos_c (pos):
    return pos[1]



# TAI move
# Lista [p_initial, p_final]
def make_move (i, f):
    return [i, f]
def move_initial (move):
    return move[0]
def move_final (move):
    return move[1]



#-------------------------------------------------------------------------------

def change_pos(board, position, piece):
    board[pos_l(position)][pos_c(position)] = piece



def numberOfPieces(board):
    pieces = 0

    # Lines
    N = len(board)

    # Columns
    M = len(board[0])

    for line in range(0, N):
        for column in range(0, M):

            if(is_peg(board[line][column])):
                pieces += 1

    return pieces



def board_moves(board):

    moves  = []

    # Lines
    N = len(board)

    # Columns
    M = len(board[0])

    for line in range(0, N):
        for column in range(0, M):
            if (is_empty(board[line][column])):

                # Vertical move
                if (line + 2 < N):
                    if (is_peg(board[line + 1][column]) and is_peg(board[line + 2][column])):
                        initial_pos = make_pos(line + 2, column)
                        final_pos = make_pos(line, column)
                        move = make_move(initial_pos, final_pos)

                        moves.append(move)

                if (line - 2 >= 0):
                    if (is_peg(board[line - 1][column]) and is_peg(board[line - 2][column])):
                        initial_pos = make_pos(line - 2, column)
                        final_pos = make_pos(line, column)
                        move = make_move(initial_pos, final_pos)

                        moves.append(move)

                # Horizontal move
                if (column + 2 < M):
                    if (is_peg(board[line][column + 1]) and is_peg(board[line][column + 2])):
                        initial_pos = make_pos(line, column + 2)
                        final_pos = make_pos(line, column)
                        move = make_move(initial_pos, final_pos)

                        moves.append(move)

                if (column - 2 >= 0):
                    if (is_peg(board[line][column - 1]) and is_peg(board[line][column - 2])):
                        initial_pos = make_pos(line, column - 2)
                        final_pos = make_pos(line, column)
                        move = make_move(initial_pos, final_pos)

                        moves.append(move)

    return moves



def board_perform_move (board, move):
    new_board = copy.deepcopy(board)

    initial_pos    = move_initial(move)
    initial_line   = pos_l(initial_pos)
    initial_column = pos_c(initial_pos)

    final_pos      = move_final(move)
    final_line     = pos_l(final_pos)
    final_column   = pos_c(final_pos)

    # Horizontal move
    if (initial_line == final_line):

        # Left to right move
        if (initial_column < final_column):
            middle_pos = make_pos(initial_line, initial_column + 1)

        # Right to left move
        else:
            middle_pos = make_pos(initial_line, initial_column - 1)

    # Vertical move
    elif (initial_column == final_column):

        # Top to bottom move
        if (initial_line < final_line):
            middle_pos = make_pos(initial_line + 1, initial_column)

        # Bottom to top move
        else:
            middle_pos = make_pos(initial_line - 1, initial_column)

    change_pos(new_board, initial_pos, c_empty())
    change_pos(new_board, middle_pos, c_empty())
    change_pos(new_board, final_pos, c_peg())

    return new_board



#-------------------------------------------------------------------------------

class sol_state:

    def __init__(self, board):
        self.board = board
        self.number_of_pieces = numberOfPieces(board)

    def __lt__(self, state):
        return self.board < state.board



def make_state(board):
    return sol_state(board)



def isolated_pegs(node):
    board = node.state.board
    
    isolated_pegs = 0
    
    # Lines
    N = len(board)
    
    # Columns
    M = len(board[0])    

    for line in range(0, N):
        for column in range(0, M):
            
            if (is_peg(board[line][column])):

                is_isolated = True
    
                # Not in the first row
                if (line != 0):
                    if (is_peg(board[line - 1][column])):
                        is_isolated = False
    
                # Not in the last row
                if(line != (N - 1)):
                    if (is_peg(board[line + 1][column])):
                        is_isolated = False
    
                # Not in the first column
                if(column != 0):
                    if (is_peg(board[line][column - 1])):
                        is_isolated = False
    
                # Not in the last column
                if(column != (M - 1)):
                    if (is_peg(board[line][column + 1])):
                        is_isolated = False
                
                if (is_isolated):
                    isolated_pegs += 1
                
    return isolated_pegs



class solitaire(Problem):

    """Models a Solitaire problem as a satisfaction problem.
    A solution cannot have more than 1 peg left on the board."""

    def __init__(self, board):
        self.initial = make_state(board)
        self.goal = 1

    def actions(self, state):
        return board_moves(state.board)

    def result(self, state, action):
        board = board_perform_move(state.board, action)
        return make_state(board)

    def goal_test(self, state):
        return state.number_of_pieces == self.goal

    def path_cost(self, c, state1, action, state2):
        return c + 1

    def h(self, node):
        return node.state.number_of_pieces**2 + isolated_pegs(node) - len(board_moves(node.state.board)) - 2

