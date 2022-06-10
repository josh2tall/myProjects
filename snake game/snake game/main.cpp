//
//  main.cpp
//  snake game
//
//  Created by Joshua Acosta on 5/15/22.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "constants.h"
#include "snake_game_core.h"

int main(int argc, const char * argv[]) {
    srand(time(0));
    int * snakePos = getSnakePositions();
    for(int i = 0; i < winningLength + 1; i++){
        snakePos[i] = -1;
    }
    
    char * currentMap = createMap();
    demoPlayer(currentMap);

    if(getGameProgress() == 'L'){
        printf("you lost!\n");
    } else {
        printf("You won!\n");
    }
    
    return 0;
}
