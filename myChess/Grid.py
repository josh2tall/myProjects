import pygame
import constants

def create_board(board):
    # draw dark first
    for x_pos in range(constants.NUM_SQUARES_PER_ROW + 1):
        for y_pos in range(constants.NUM_SQUARES_PER_ROW + 1):
            color = constants.DARK
            if x_pos % 2 == 0 and y_pos % 2 != 0:
                color = constants.LIGHT
            elif x_pos % 2 != 0 and y_pos % 2 == 0:
                color = constants.LIGHT
            x_coord, y_coord = calculate_square_coordinates(x_pos, y_pos)
            pygame.draw.rect(board, color, pygame.Rect(x_coord, y_coord, constants.BLOCK_SIZE, constants.BLOCK_SIZE))

def calculate_square_coordinates(coordinate_x, coordinate_y):
    return (coordinate_x - 1) * constants.BLOCK_SIZE, (coordinate_y - 1) * constants.BLOCK_SIZE

def show_pieces(board, current_locations, text_module):
    for row in range(constants.NUM_SQUARES_PER_ROW):
        for column in range(constants.NUM_SQUARES_PER_ROW):
            if current_locations[column][row] != 'e':
                text_surface = text_module.render(current_locations[column][row], False, (0, 0, 0))
                text_height = text_surface.get_rect().height
                text_width = text_surface.get_rect().width
                x_coord, y_coord = calculate_piece_coordinates(row, column, text_height, text_width)
                board.blit(text_surface, (y_coord, x_coord))

def calculate_piece_coordinates(row, column, text_height, text_width):
    return constants.SQUARE_SIZE * row + constants.SQUARE_SIZE / 2 - text_height / 2, \
           constants.SQUARE_SIZE * column + constants.SQUARE_SIZE / 2 - text_width / 2

def determine_block_click(board):
    info = []
    pos = pygame.mouse.get_pos()
    block_column = int(pos[0] / constants.SQUARE_SIZE)
    block_row = int(pos[1] / constants.SQUARE_SIZE)

    info.append(block_column)
    info.append(block_row)

    print("clicked: column: ", block_column, ", row: ", block_row, ", contains: ", board[block_column][block_row])
    return info

def highlight_block(x_block, y_block, board, screen):
    coord = convert_row_column_to_coord(x_block, y_block)
    color = constants.LIGHT
    current_color = screen.get_at((coord[0], coord[1]))
    if current_color[0] == constants.LIGHT[0] and current_color[1] == constants.LIGHT[1]\
            and current_color[2] == constants.LIGHT[2]:
        color = constants.HIGHLIGHTED_DARK
    elif current_color[0] == constants.DARK[0] and current_color[2] == constants.DARK[2]\
            and current_color[2] == constants.DARK[2]:
        color = constants.HIGHLIGHTED_LIGHT
    elif current_color[0] == constants.HIGHLIGHTED_LIGHT[0] and current_color[1] == constants.HIGHLIGHTED_LIGHT[1]\
            and current_color[2] == constants.HIGHLIGHTED_LIGHT[2]:
        color = constants.DARK
    else:
        color = constants.LIGHT

    pygame.draw.rect(screen, color, pygame.Rect(coord[0], coord[1], constants.BLOCK_SIZE, constants.BLOCK_SIZE))


def create_single_piece(row, column, board, text_module, screen):
    text_surface = text_module.render(board[row][column], False, (0, 0, 0))
    text_height = text_surface.get_rect().height
    text_width = text_surface.get_rect().width
    x_coord = constants.SQUARE_SIZE * column + constants.SQUARE_SIZE / 2 - text_height / 2
    y_coord = constants.SQUARE_SIZE * row + constants.SQUARE_SIZE / 2 - text_width / 2
    screen.blit(text_surface, (y_coord, x_coord))

def convert_row_column_to_coord(row, column):
    x_coord = int(row * constants.BLOCK_SIZE)
    y_coord = int(column * constants.BLOCK_SIZE)

    return x_coord, y_coord

def convert_coord_to_row_column(x_coord, y_coord):
    row = int(x_coord / constants.BLOCK_SIZE)
    column = int(y_coord / constants.BLOCK_SIZE)

    return row, column

def refresh_board(screen, board, font_module):
    create_board(screen)
    show_pieces(screen, board, font_module)
