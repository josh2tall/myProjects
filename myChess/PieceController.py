import constants


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
