# This is a sample Python script.

# Press ⌃R to execute it or replace it with your code.
# Press Double ⇧ to search everywhere for classes, files, tool windows, actions, and settings.

import Grid
import PieceController
import pygame
import constants

# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    pygame.init()
    my_font = pygame.font.SysFont('arial', 30)

    Board = PieceController.create_pieces()

    PieceController.show_board(Board)

    screen = pygame.display.set_mode((constants.WIDTH, constants.HEIGHT))

    done = False

    while not done:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                done = True
        Grid.create_board(screen)
        Grid.show_pieces(screen, Board, my_font)
        pygame.display.flip()


# See PyCharm help at https://www.jetbrains.com/help/pycharm/
