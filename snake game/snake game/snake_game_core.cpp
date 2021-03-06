//
//  snake_game_core.cpp
//  snake game
//
//  Created by Joshua Acosta on 6/5/22.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "constants.h"
#include "snake_game_core.h"

char currentDirection = defaultDirection;
char gameProgress = 'P';

int snakeLength = startingSnakeLength;
int snakePositions[snakePositionsLength];

char getGameProgress(void){
    return gameProgress;
}

int * getSnakePositions(void){
    return snakePositions;
}

void resetGame(char * map){
    //clear map
    for(int i = 0; i < MAP_SIZE; i++){
        map[i] = emptySpace;
    }
    
    gameProgress = 'P';
    snakeLength = startingSnakeLength;
    currentDirection = defaultDirection;

    snakePositions[snakeHeadPosition] = MAP_SIZE / 2;
    for(int i = 1; i < snakePositionsLength; i++){
        snakePositions[i] = emptySpace;
    }
}

void useInputToChangeDirection(void){
    bool retry = true;
    while(retry){
        char c = getchar();
        
        if(c == 'a'){
            currentDirection = 'W';
            retry = false;
        } else if(c == 's'){
            currentDirection = 'S';
            retry = false;
        } else if(c == 'd'){
            currentDirection = 'E';
            retry = false;
        } else if(c == 'w'){
            currentDirection = 'N';
            retry = false;
        }
    }
}

char * createMap(void){
    char * Map = (char*) malloc(MAP_SIZE * sizeof(char));
    
    for(int i = 0; i < MAP_SIZE; i++){
        Map[i] = emptySpace;
    }
    
    return Map;
}

void addSnakeHead(char * map){
    map[MAP_SIZE / 2] = snakeHead;
    snakePositions[snakeHeadPosition] = MAP_SIZE / 2;
}

void addRandomFood(char * map){
    bool tryAgain = true;
    
    while(tryAgain){
        int num = (rand() %
                   ((MAP_SIZE)));
        if(map[num] == emptySpace){
            map[num] = food;
            tryAgain = false;
        }
    }
}

void renderScreen(char * currentMap){
    for(int i = 0; i < MAP_SIZE; i++){
        if(i%MAP_LENGTH == 0){
            printf("|");
        }
        
        char temp = currentMap[i];
        printf("%-1c", temp);
        
        if(i%MAP_LENGTH == MAP_LENGTH - 1){
            printf("|\r\n");
        }
        
    }
    printf("\r\n");
    
}

bool changeDirection(char newDirection){
    if(newDirection == 'N' || newDirection == 'S' || newDirection == 'W' || newDirection == 'E'){
        currentDirection = newDirection;
        return true;
    }
    
    return false;
}

void step(char * map){
    int oldSnakeLocation = snakePositions[snakeHeadPosition];
    int newSnakeLocation = oldSnakeLocation;
    
    if(oldSnakeLocation < 0 && oldSnakeLocation >= MAP_SIZE){
        gameProgress = 'L';
    } else if(currentDirection == 'N'){
        if(oldSnakeLocation - MAP_LENGTH >= 0){
            newSnakeLocation -= MAP_LENGTH;
        } else {
            gameProgress = 'L';
        }
    } else if(currentDirection == 'S'){
        if(oldSnakeLocation + MAP_LENGTH < MAP_SIZE){
            newSnakeLocation += MAP_LENGTH;
        } else {
            gameProgress = 'L';
        }
    } else if(currentDirection == 'W'){
        if(oldSnakeLocation%MAP_LENGTH - 1 >= 0){
            newSnakeLocation -= 1;
        } else {
            gameProgress = 'L';
        }
    } else if(currentDirection == 'E'){
        if(oldSnakeLocation%MAP_LENGTH + 1 < MAP_LENGTH){
            newSnakeLocation += 1;
        } else {
            gameProgress = 'L';
        }
    }
    
    if(newSnakeLocation != oldSnakeLocation){
        char newLocation = map[newSnakeLocation];
        if(newLocation == emptySpace){
            int temp1 = newSnakeLocation;
            int temp2;
            for(int i = 0; i < snakeLength; i++){
                // +1 for head
                temp2 = snakePositions[i];
                map[temp2] = emptySpace;
                snakePositions[i] = temp1;
                if(i == 0){
                    map[temp1] = snakeHead;
                } else {
                    map[temp1] = snakeBody;
                }
                temp1 = temp2;
            }
        } else if(newLocation == food){
            int temp1 = newSnakeLocation;
            int temp2;
            for(int i = 0; i < snakeLength + 1; i++){
                // +1 for head
                temp2 = snakePositions[i];
                map[temp2] = emptySpace;
                snakePositions[i] = temp1;
                if(i == 0){
                    map[temp1] = snakeHead;
                } else {
                    map[temp1] = snakeBody;
                }
                temp1 = temp2;
            }
            addRandomFood(map);
            snakeLength++;
        } else if(newLocation == snakeBody){
            gameProgress = 'L';
        }
    }
    
    if(snakeLength == winningLength){
        gameProgress = 'W';
    }
}

void demoPlayer(char * currentMap){
    addSnakeHead(currentMap);
    addRandomFood(currentMap);
    bool keepGoing = true;
    changeDirection('S');
    while(keepGoing && gameProgress == 'P'){
        renderScreen(currentMap);
        useInputToChangeDirection();
        step(currentMap);
    }
}
