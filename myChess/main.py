# This is a sample Python script.

# Press ⌃R to execute it or replace it with your code.
# Press Double ⇧ to search everywhere for classes, files, tool windows, actions, and settings.

import Grid
import PieceController
import pygame

# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    pygame.font.init()  # you have to call this at the start
    my_font = pygame.font.SysFont('arial', 30)

    Board = PieceController.create_pieces()

    PieceController.show_board(Board)

    Grid.create_display(Board, my_font)


# See PyCharm help at https://www.jetbrains.com/help/pycharm/
