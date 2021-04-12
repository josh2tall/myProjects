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
