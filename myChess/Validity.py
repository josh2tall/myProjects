import constants

def determine_move_validity(source_block, destination_block, board, is_white_turn):
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

    if current_piece == 'r' or current_piece == "R":
        valid_moves = get_rook_valid_moves(source_block)
        if (destination_block[0], destination_block[1]) not in valid_moves:
            validity = False
            print("move is not in list of valid moves.")

    if current_piece == 'n' or current_piece == "N":
        valid_moves = get_knight_valid_moves(source_block)
        if (destination_block[0], destination_block[1]) not in valid_moves:
            validity = False
            print("move is not in list of valid moves.")

    if current_piece == 'b' or current_piece == "B":
        valid_moves = get_bishop_valid_moves(source_block, board)
        print("destination_block ", destination_block)
        if (destination_block[0], destination_block[1]) not in valid_moves:
            validity = False
            print("move is not in list of valid moves.")

    return validity


def determine_piece_validity(new_move, board, is_white_turn):
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


def get_rook_valid_moves(current_pos):
    valid_moves = []

    column = 0
    row = 1

    # get all moves to the right
    for block in range(constants.NUM_SQUARES_PER_ROW):
        if block != current_pos[column]:
            valid_moves.append((block, current_pos[row]))

    for block in range(constants.NUM_SQUARES_PER_ROW):
        if block != current_pos[row]:
            valid_moves.append((current_pos[column], block))

    return valid_moves


def get_knight_valid_moves(current_pos):
    valid_moves = []

    column = 0
    row = 1

    if current_pos[row] - 2 >= 0:
        if current_pos[column] - 1 >= 0:
            valid_moves.append((current_pos[column] - 1, current_pos[row] - 2))
        if current_pos[column] + 1 <= constants.NUM_SQUARES_PER_ROW - 1:
            valid_moves.append((current_pos[column] + 1, current_pos[row] - 2))

    if current_pos[row] - 1 >= 0:
        if current_pos[column] - 2 >= 0:
            valid_moves.append((current_pos[column] - 2, current_pos[row] - 1))
        if current_pos[column] + 2 <= constants.NUM_SQUARES_PER_ROW - 1:
            valid_moves.append((current_pos[column] + 2, current_pos[row] - 1))

    if current_pos[row] + 1 >= 0:
        if current_pos[column] - 2 >= 0:
            valid_moves.append((current_pos[column] - 2, current_pos[row] + 1))
        if current_pos[column] + 2 <= constants.NUM_SQUARES_PER_ROW - 1:
            valid_moves.append((current_pos[column] + 2, current_pos[row] + 1))

    if current_pos[row] + 2 >= 0:
        if current_pos[column] - 1 >= 0:
            valid_moves.append((current_pos[column] - 1, current_pos[row] + 2))
        if current_pos[column] + 1 <= constants.NUM_SQUARES_PER_ROW - 1:
            valid_moves.append((current_pos[column] + 1, current_pos[row] + 2))

    return valid_moves


def get_bishop_valid_moves(current_pos, board):
    valid_moves = []

    column = 0
    row = 1
    done = False
    iterator = 1

    current_piece = board[current_pos[column]][current_pos[row]]

    top_right_done = False
    top_left_done = False
    bottom_right_done = False
    bottom_left_done = False

    while not done:
        found_blocks = False

        # top right
        if current_pos[row] - iterator >= 0 and \
                current_pos[column] + iterator <= constants.NUM_SQUARES_PER_ROW - 1 and not top_right_done:
            potential_move = [current_pos[column] + iterator, current_pos[row] - iterator]
            list_potential_move = current_pos[column] + iterator, current_pos[row] - iterator
            piece_at_potential_move = board[potential_move[column]][potential_move[row]]
            if piece_at_potential_move == 'e':
                valid_moves.append(list_potential_move)
            elif is_same_type(piece_at_potential_move, current_piece):
                top_right_done = True
            else:
                valid_moves.append(list_potential_move)
                top_right_done = True
            found_blocks = True
        # top left
        if current_pos[row] - iterator >= 0 and current_pos[column] - iterator >= 0 and not top_left_done:
            potential_move = [current_pos[column] - iterator, current_pos[row] - iterator]
            list_potential_move = current_pos[column] - iterator, current_pos[row] - iterator
            piece_at_potential_move = board[potential_move[column]][potential_move[row]]
            if piece_at_potential_move == 'e':
                valid_moves.append(list_potential_move)
            elif is_same_type(piece_at_potential_move, current_piece):
                top_left_done = True
            else:
                valid_moves.append(list_potential_move)
                top_left_done = True
            found_blocks = True
        # bottom right
        if current_pos[row] + iterator <= constants.NUM_SQUARES_PER_ROW - 1 and \
                current_pos[column] + iterator <= constants.NUM_SQUARES_PER_ROW - 1 and not bottom_right_done:
            potential_move = [current_pos[column] + iterator, current_pos[row] + iterator]
            list_potential_move = current_pos[column] + iterator, current_pos[row] + iterator
            piece_at_potential_move = board[potential_move[column]][potential_move[row]]
            if piece_at_potential_move == 'e':
                valid_moves.append(list_potential_move)
            elif is_same_type(piece_at_potential_move, current_piece):
                bottom_right_done = True
            else:
                valid_moves.append(list_potential_move)
                bottom_right_done = True
            found_blocks = True
        # bottom left
        if current_pos[row] + iterator <= constants.NUM_SQUARES_PER_ROW - 1 and \
                current_pos[column] - iterator >= 0 and not bottom_left_done:
            potential_move = [current_pos[column] - iterator, current_pos[row] + iterator]
            list_potential_move = current_pos[column] - iterator, current_pos[row] + iterator
            piece_at_potential_move = board[potential_move[column]][potential_move[row]]
            if piece_at_potential_move == 'e':
                valid_moves.append(list_potential_move)
            elif is_same_type(piece_at_potential_move, current_piece):
                bottom_left_done = True
            else:
                valid_moves.append(list_potential_move)
                bottom_left_done = True
            found_blocks = True

        print("iterator ", iterator)
        iterator = iterator + 1

        if not found_blocks:
            done = True

    print("bishop valid moves", valid_moves)
    return valid_moves

def is_same_type(piece_a, piece_b):
    return (piece_a.islower() and piece_b.islower()) or (piece_a.isupper() and piece_b.isupper())
