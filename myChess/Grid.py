import pygame
import constants

def create_board(board):
    # draw dark first
    for x_pos in range(constants.NUM_SQUARES_PER_ROW + 1):
        for y_pos in range(constants.NUM_SQUARES_PER_ROW + 1):
            color = constants.GREEN
            if x_pos % 2 == 0 and y_pos % 2 != 0:
                color = constants.BLUE
            elif x_pos % 2 != 0 and y_pos % 2 == 0:
                color = constants.BLUE
            x_coord = (x_pos - 1) * constants.BLOCK_SIZE
            y_coord = (y_pos - 1) * constants.BLOCK_SIZE
            pygame.draw.rect(board, color, pygame.Rect(x_coord, y_coord, constants.BLOCK_SIZE, constants.BLOCK_SIZE))


def show_pieces(board, current_locations, text_module):
    for row in range(constants.NUM_SQUARES_PER_ROW):
        for column in range(constants.NUM_SQUARES_PER_ROW):
            if current_locations[column][row] != 'e':
                text_surface = text_module.render(current_locations[column][row], False, (0, 0, 0))
                text_height = text_surface.get_rect().height
                text_width = text_surface.get_rect().width
                x_coord = constants.SQUARE_SIZE * row + constants.SQUARE_SIZE / 2 - text_height / 2
                y_coord = constants.SQUARE_SIZE * column + constants.SQUARE_SIZE / 2 - text_width / 2
                board.blit(text_surface, (y_coord, x_coord))


def determine_block_click(pos, board):
    info = []
    block_column = int(pos[0] / constants.SQUARE_SIZE)
    block_row = int(pos[1] / constants.SQUARE_SIZE)

    info.append(block_column)
    info.append(block_row)
    info.append(board[block_column][block_row])

    print("clicked: column: ", block_column, ", row: ", block_row, ", contains: ", board[block_column][block_row])
    return info

def highlight_block(x_block, y_block, board, screen):
    coord = convert_row_column_to_coord(x_block, y_block)
    color = constants.BLUE
    print(x_block, y_block, coord)
    current_color = screen.get_at((coord[0], coord[1]))
    if current_color[0] == constants.BLUE[0] and current_color[2] == constants.BLUE[2]:
        color = constants.HIGHLIGHTED_BLUE
        print("changing to highlighted blue")
    elif current_color[0] == constants.GREEN[0] and current_color[2] == constants.GREEN[2]:
        color = constants.HIGHLIGHTED_GREEN
        print("changing to highlighted green")
    elif current_color[0] == constants.HIGHLIGHTED_GREEN[0] and current_color[2] == constants.HIGHLIGHTED_GREEN[2]:
        color = constants.GREEN
        print("changing to green")
    else:
        color = constants.BLUE
        print("changing to blue")

    pygame.draw.rect(screen, color, pygame.Rect(coord[0], coord[1], constants.BLOCK_SIZE, constants.BLOCK_SIZE))


def create_single_piece(row, column, board, text_module, screen):
    text_surface = text_module.render(board[row][column], False, (0, 0, 0))
    text_height = text_surface.get_rect().height
    text_width = text_surface.get_rect().width
    x_coord = constants.SQUARE_SIZE * column + constants.SQUARE_SIZE / 2 - text_height / 2
    y_coord = constants.SQUARE_SIZE * row + constants.SQUARE_SIZE / 2 - text_width / 2
    print(board[row][column])
    print("x coord: ", x_coord)
    print("y coord: ", y_coord)
    screen.blit(text_surface, (y_coord, x_coord))

def convert_row_column_to_coord(row, column):
    x_coord = int(row * constants.BLOCK_SIZE)
    y_coord = int(column * constants.BLOCK_SIZE)

    return x_coord, y_coord

def convert_coord_to_row_column(x_coord, y_coord):
    row = int(x_coord / constants.BLOCK_SIZE)
    column = int(y_coord / constants.BLOCK_SIZE)

    return row, column
