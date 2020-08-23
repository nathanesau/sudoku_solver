# to see print output, use "py.test -s"
import pytest
import fast_solve

def read_board(file_name):
    f = open(file_name, 'r')
    data = f.read()
    f.close()
    board = [[[] for i in range(9)] for i in range(9)]
    for row, line in enumerate(data.splitlines()):
        for col, value in enumerate(line.split(',')):
            if value != '0':
                board[row][col] = [int(value)]
    return board

def print_board(board):
    for i in range(9):
        for j in range(9):
            if len(board[i][j]) == 1:
                print(board[i][j][0], end= " ")
            else:
                print("X", end=" ")
        print("")

def validate_solve(board):
    for i in range(9):
        rnum, cnum, bnum = [], [], []
        for j in range(9):
             rnum += board[i][j]
             cnum += board[i][j]
             bnum += board[i][j]
        assert(sorted(rnum) == [1,2,3,4,5,6,7,8,9])
        assert(sorted(cnum) == [1,2,3,4,5,6,7,8,9])
        assert(sorted(bnum) == [1,2,3,4,5,6,7,8,9])

def test_solve_easy():
    initial_board = read_board("easy_board.csv")
    print("\ninitial board")
    print_board(initial_board)
    solved_board = fast_solve.solve(initial_board)
    print("\nsolved board")
    print_board(solved_board)
    validate_solve(solved_board)

def test_solve_medium():
    initial_board_1 = read_board("medium_board_1.csv")
    print("\nmedium board 1")
    print_board(initial_board_1)
    solved_board_1 = fast_solve.solve(initial_board_1)
    print("\nsolved board 1")
    print_board(solved_board_1)
    validate_solve(solved_board_1)
    initial_board_2 = read_board("medium_board_2.csv")
    print("\nmedium board 2")
    print_board(initial_board_2)
    solved_board_2 = fast_solve.solve(initial_board_2)
    print("\nsolved board 2")
    print_board(solved_board_2)
    validate_solve(solved_board_2)

def test_solve_hard():
    initial_board = read_board("hard_board.csv")
    print("\ninitial board")
    print_board(initial_board)
    solved_board = fast_solve.solve(initial_board)
    print("\nsolved board")
    print_board(solved_board)
    validate_solve(solved_board)