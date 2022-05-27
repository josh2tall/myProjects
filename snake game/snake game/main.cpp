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

const int MAP_SIZE = 11;
char currentDirection = 'N';
char gameProgress = 'P';

const char snakeHead = '^';
const char emptySpace = '_';
const char food = '$';
const char snakeBody = ';';

int snakeLength = 1;
const int winningLength = 3;

int snakePositions[winningLength + 1];
int snakePositionLength = 1;
const int snakeHeadPosition = 0;


char * createMap(void){
    char * Map = (char*) malloc(MAP_SIZE * MAP_SIZE * sizeof(char));
    
    for(int i = 0; i < MAP_SIZE * MAP_SIZE; i++){
        Map[i] = emptySpace;
    }
    
    return Map;
}

void addSnakeHead(char * map){
    map[MAP_SIZE * MAP_SIZE / 2] = snakeHead;
    snakePositions[snakeHeadPosition] = MAP_SIZE * MAP_SIZE / 2;
}

void addRandomFood(char * map){
    bool tryAgain = true;
    
    while(tryAgain){
        int num = (rand() %
                   ((MAP_SIZE * MAP_SIZE)));
        if(map[num] == emptySpace){
            map[num] = food;
            tryAgain = false;
        }
    }
}

void renderScreen(char * currentMap){
    for(int i = 0; i < MAP_SIZE * MAP_SIZE; i++){
        if(i%MAP_SIZE == 0){
            printf("|");
        }
        
        char temp = currentMap[i];
        printf("%-1c", temp);
        
        if(i%MAP_SIZE == MAP_SIZE - 1){
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
    
    if(oldSnakeLocation < 0 && oldSnakeLocation >= MAP_SIZE * MAP_SIZE){
        gameProgress = 'L';
    } else if(currentDirection == 'N'){
        if(oldSnakeLocation - MAP_SIZE >= 0){
            newSnakeLocation -= MAP_SIZE;
        } else {
            gameProgress = 'L';
        }
    } else if(currentDirection == 'S'){
        if(oldSnakeLocation + MAP_SIZE < MAP_SIZE * MAP_SIZE){
            newSnakeLocation += MAP_SIZE;
        } else {
            gameProgress = 'L';
        }
    } else if(currentDirection == 'W'){
        if(oldSnakeLocation%MAP_SIZE - 1 >= 0){
            newSnakeLocation -= 1;
        } else {
            gameProgress = 'L';
        }
    } else if(currentDirection == 'E'){
        if(oldSnakeLocation%MAP_SIZE + 1 < MAP_SIZE){
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
            for(int i = 0; i < snakePositionLength; i++){
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
            for(int i = 0; i < snakePositionLength + 1; i++){
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
            snakePositionLength++;
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
