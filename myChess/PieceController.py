import constants
import pygame
import Grid

last_click = [-1, -1]

def create_pieces():
    # create an empty board
    pieces_on_board = [['e' for _ in range(constants.NUM_SQUARES_PER_ROW)] for _ in
                       range(constants.NUM_SQUARES_PER_ROW)]

    # 8 pawns, 2 rooks, 2 knights, 2 bishops, 1 queen, 1 king
    # per side

    # create pawns
    for column in range(constants.NUM_SQUARES_PER_ROW):
        pieces_on_board[column][1] = 'p'
        pieces_on_board[column][6] = 'p'

    side_order = [0, 7]
    # top side first
    for x in side_order:
        # create rooks
        pieces_on_board[0][x] = 'r'
        pieces_on_board[7][x] = 'r'

        # create knights
        pieces_on_board[1][x] = 'n'
        pieces_on_board[6][x] = 'n'

        # create bishops
        pieces_on_board[2][x] = 'b'
        pieces_on_board[5][x] = 'b'

        # create queen
        pieces_on_board[3][x] = 'q'

        # create king
        pieces_on_board[4][x] = 'k'

    return pieces_on_board


def show_board(board_with_pieces):
    for x in range(constants.NUM_SQUARES_PER_ROW):
        row = []
        for y in range(constants.NUM_SQUARES_PER_ROW):
            row.append(board_with_pieces[y][x])
        print(row)

def move_piece(last_move, new_move, board):
    piece_last_move = board[last_move[0]][last_move[1]]
    if piece_last_move != 'e':
        board[new_move[0]][new_move[1]] = piece_last_move
        board[last_move[0]][last_move[1]] = 'e'

    return board

def process_click(board, screen, font):
    global last_click
    pos = pygame.mouse.get_pos()
    new_move = Grid.determine_block_click(pos, board)
    if last_click[0] == new_move[0] and last_click[1] == new_move[1]:
        last_click = [-1, -1]
        Grid.create_board(screen)
        Grid.show_pieces(screen, board, font)
    elif last_click[0] >= 0 and last_click[1] >= 0:
        # move last click to detailed info
        board = move_piece(last_click, new_move, board)
        last_click[0] = -1
        last_click[1] = -1
        Grid.create_board(screen)
        Grid.show_pieces(screen, board, font)
    elif board[new_move[0]][new_move[1]] != 'e':
        last_click[0] = new_move[0]
        last_click[1] = new_move[1]
        x_block = last_click[0]
        y_block = last_click[1]
        Grid.highlight_block(x_block, y_block, board, screen)
        Grid.create_single_piece(x_block, y_block, board, font, screen)

    return board
