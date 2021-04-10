
import pygame
import constants

def create_board(board):
    # draw dark first
    blue = (0, 128, 255)
    green = (0, 128, 100)
    block_size = constants.WIDTH / 8

    for x_pos in range(constants.NUM_SQUARES_PER_ROW + 1):
        for y_pos in range(constants.NUM_SQUARES_PER_ROW + 1):
            color = green
            if x_pos % 2 == 0 and y_pos % 2 != 0:
                color = blue
            elif x_pos % 2 != 0 and y_pos % 2 == 0:
                color = blue
            x_coord = (x_pos - 1) * block_size
            y_coord = (y_pos - 1) * block_size
            pygame.draw.rect(board, color, pygame.Rect(x_coord, y_coord, block_size, block_size))


def show_pieces(board, current_locations, text_module):
    for row in range(constants.NUM_SQUARES_PER_ROW):
        for column in range(constants.NUM_SQUARES_PER_ROW):
            if current_locations[column][row] != 'e':
                text_surface = text_module.render(current_locations[column][row], False, (0, 0, 0))
                text_height = text_surface.get_rect().height
                text_width = text_surface.get_rect().width
                x_coord = constants.SQUARE_SIZE * row + constants.SQUARE_SIZE/2 - text_height / 2
                y_coord = constants.SQUARE_SIZE * column + constants.SQUARE_SIZE / 2 - text_width / 2
                board.blit(text_surface, (y_coord, x_coord))
