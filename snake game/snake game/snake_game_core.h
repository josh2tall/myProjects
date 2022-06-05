//
//  snake_game_core.h
//  snake game
//
//  Created by Joshua Acosta on 6/5/22.
//

#ifndef snake_game_core_h
#define snake_game_core_h

/**
    Returns the snake positions array
 */
int * getSnakePositions(void);

/**
    Creates and returns an empty snake map based on MAP_SIZE variable
 */
char * createMap(void);

/**
    Creates the snake head and food and allows a player to play snake
 */
void demoPlayer(char * currentMap);

/**
    Returns if the game has ended or is ongoing
 */
char getGameProgress(void);

#endif /* snake_game_core_h */
