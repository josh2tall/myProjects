import constants
import pygame
import Grid

is_white_turn = True
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
        pieces_on_board[column][6] = 'P'

    # create rooks
    pieces_on_board[0][0] = 'r'
    pieces_on_board[7][0] = 'r'

    # create knights
    pieces_on_board[1][0] = 'n'
    pieces_on_board[6][0] = 'n'

    # create bishops
    pieces_on_board[2][0] = 'b'
    pieces_on_board[5][0] = 'b'

    # create queen
    pieces_on_board[3][0] = 'q'

    # create king
    pieces_on_board[4][0] = 'k'

    # create bottom
    # create rooks
    pieces_on_board[0][7] = 'R'
    pieces_on_board[7][7] = 'R'

    # create knights
    pieces_on_board[1][7] = 'N'
    pieces_on_board[6][7] = 'N'

    # create bishops
    pieces_on_board[2][7] = 'B'
    pieces_on_board[5][7] = 'B'

    # create queen
    pieces_on_board[3][7] = 'Q'

    # create king
    pieces_on_board[4][7] = 'K'

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
    global is_white_turn
    pos = pygame.mouse.get_pos()
    new_move = Grid.determine_block_click(pos, board)

    print("is it white's turn: ", is_white_turn)
    is_valid_move = determine_move_validity(last_click, new_move, board)
    can_select_piece = determine_piece_validity(new_move, board)
    if last_click[0] == new_move[0] and last_click[1] == new_move[1]:
        last_click = [-1, -1]
        Grid.highlight_block(new_move[0], new_move[1], board, screen)
        Grid.create_single_piece(new_move[0], new_move[1], board, font, screen)
    elif last_click[0] >= 0 and last_click[1] >= 0:
        # move last click to detailed info
        if is_valid_move:
            board = move_piece(last_click, new_move, board)
            is_white_turn = not is_white_turn
            if is_white_turn:
                pygame.display.set_caption('Turn: White')
            else:
                pygame.display.set_caption('Turn: Black')

        Grid.create_board(screen)
        Grid.show_pieces(screen, board, font)
        last_click[0] = -1
        last_click[1] = -1
    elif board[new_move[0]][new_move[1]] != 'e' and can_select_piece:
        last_click[0] = new_move[0]
        last_click[1] = new_move[1]
        x_block = last_click[0]
        y_block = last_click[1]
        Grid.highlight_block(x_block, y_block, board, screen)
        Grid.create_single_piece(x_block, y_block, board, font, screen)

    pygame.display.flip()
    return board

def determine_move_validity(source_block, destination_block, board):
    destination_piece = board[destination_block[0]][destination_block[1]]
    current_piece = board[source_block[0]][source_block[1]]
    validity = True

    if destination_piece != 'e':
        if is_white_turn and destination_piece.isupper():
            validity = False
        elif not is_white_turn and destination_piece.islower():
            validity = False

    if current_piece == 'k' or current_piece == 'K':
        valid_moves = get_king_valid_moves(source_block)
        print("destination", destination_block)
        if (destination_block[0], destination_block[1]) not in valid_moves:
            validity = False
            print("move is not in list of valid moves.")

    return validity

def determine_piece_validity(new_move, board):
    validity = True
    piece = board[new_move[0]][new_move[1]]

    if piece.isupper() and not is_white_turn or piece.islower() and is_white_turn:
        validity = False

    return validity

def get_king_valid_moves(current_pos):
    valid_moves = []

    column = 0
    row = 1

    # check top left
    if current_pos[column] - 1 >= 0 and current_pos[row] - 1 >= 0:
        valid_moves.append((current_pos[column] - 1, current_pos[row] - 1))

    # check top
    if current_pos[row] - 1 >= 0:
        valid_moves.append((current_pos[column], current_pos[row] - 1))

    # check top right
    if current_pos[column] + 1 <= constants.NUM_SQUARES_PER_ROW - 1 and current_pos[row] - 1 >= 0:
        valid_moves.append((current_pos[column] + 1, current_pos[row] - 1))

    # check right
    if current_pos[column] + 1 <= constants.NUM_SQUARES_PER_ROW - 1:
        valid_moves.append((current_pos[column] + 1, current_pos[row]))

    # check bottom right
    if current_pos[column] + 1 <= constants.NUM_SQUARES_PER_ROW - 1 and \
       current_pos[row] + 1 <= constants.NUM_SQUARES_PER_ROW - 1:
        valid_moves.append((current_pos[column] + 1, current_pos[row] + 1))

    # check bottom
    if current_pos[row] + 1 <= constants.NUM_SQUARES_PER_ROW - 1:
        valid_moves.append((current_pos[column], current_pos[row] + 1))

    # check bottom left
    if current_pos[column] - 1 >= 0 and current_pos[row] + 1 <= constants.NUM_SQUARES_PER_ROW - 1:
        valid_moves.append((current_pos[column] - 1, current_pos[row] + 1))

    # check left
    if current_pos[column] - 1 >= 0:
        valid_moves.append((current_pos[column] - 1, current_pos[row]))

    print("valid_moves", valid_moves)
    return valid_moves
