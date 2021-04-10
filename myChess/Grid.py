
import pygame

WIDTH = 400
HEIGHT = 400
NUM_SQUARES_PER_ROW = 8

def create_display():
    pygame.init()
    screen = pygame.display.set_mode((WIDTH, HEIGHT))

    done = False

    while not done:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                done = True
        create_board(screen)
        pygame.display.flip()

def create_board(board):
    # draw dark first
    blue = (0, 128, 255)
    green = (0, 128, 100)
    block_size = WIDTH/8

    for x_pos in range(NUM_SQUARES_PER_ROW + 1):
        for y_pos in range(NUM_SQUARES_PER_ROW + 1):
            color = green
            if x_pos % 2 == 0 and y_pos % 2 != 0:
                color = blue
            elif x_pos % 2 != 0 and y_pos % 2 == 0:
                color = blue
            x_coord = (x_pos - 1) * block_size
            y_coord = (y_pos - 1) * block_size
            pygame.draw.rect(board, color, pygame.Rect(x_coord, y_coord, block_size, block_size))
