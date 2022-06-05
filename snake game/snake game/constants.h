//
//  constants.h
//  snake game
//
//  Created by Joshua Acosta on 6/5/22.
//

#ifndef constants_h
#define constants_h

const int MAP_LENGTH = 11;
const int MAP_SIZE = MAP_LENGTH * MAP_LENGTH;
const int winningLength = 5;
const int snakeHeadPosition = 0;
const int startingSnakeLength = 1;
const int snakePositionsLength = winningLength + 1;

const char snakeHead = '^';
const char emptySpace = '_';
const char food = '$';
const char snakeBody = ';';
const char defaultDirection = 'N';

#endif /* constants_h */
