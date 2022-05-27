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

const int MAP_LENGTH = 11;
const int MAP_SIZE = MAP_LENGTH * MAP_LENGTH;
const int winningLength = 3;
const int snakeHeadPosition = 0;
const int startingSnakeLength = 1;
const int snakePositionsLength = winningLength + 1;

const char snakeHead = '^';
const char emptySpace = '_';
const char food = '$';
const char snakeBody = ';';

char currentDirection = 'N';
char gameProgress = 'P';

int snakeLength = startingSnakeLength;
int snakePositions[snakePositionsLength];


void resetGame(char * map){
    //clear map
    for(int i = 0; i < MAP_SIZE; i++){
        map[i] = emptySpace;
    }
    
    gameProgress = 'P';
    snakeLength = startingSnakeLength;
    
    snakePositions[snakeHeadPosition] = MAP_SIZE / 2;
    for(int i = 1; i < snakePositionsLength; i++){
        snakePositions[i] = emptySpace;
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

void changeDirection(char newDirection){
    if(newDirection == 'N' || newDirection == 'S' || newDirection == 'W' || newDirection == 'E'){
        currentDirection = newDirection;
    }
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
        }
        else if(newLocation == food){
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
        }
    }
    
    if(snakeLength == winningLength){
        gameProgress = 'W';
    }
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    // Use current time as
    // seed for random generator
    srand(time(0));
    
    for(int i = 0; i < winningLength + 1; i++){
        snakePositions[i] = -1;
    }
    
    char * currentMap = createMap();
    addSnakeHead(currentMap);
    addRandomFood(currentMap);
    bool keepGoing = true;
    changeDirection('S');
    while(keepGoing && gameProgress == 'P'){
        renderScreen(currentMap);
        step(currentMap);
    }
    
    if(gameProgress == 'L'){
        printf("you lost!");
    } else {
        printf("You won!");
    }
    printf("length is %d", snakeLength);
    for(int i = 0; i < snakeLength + 1; i++){
        printf("array[%i] is %i", i, snakePositions[i]);
    }
    
    return 0;
}
