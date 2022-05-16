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

char * createMap(void){
    char * Map = (char*) malloc(MAP_SIZE * MAP_SIZE * sizeof(char));
    
    for(int i = 0; i < MAP_SIZE * MAP_SIZE; i++){
        char temp = '_';
        Map[i] = temp;
    }
    
    return Map;
}

void addSnakeHead(char * map){
    map[MAP_SIZE * MAP_SIZE / 2] = '^';
}

void addRandomFood(char * map){
    bool tryAgain = true;
    
    while(tryAgain){
        int num = (rand() %
                   ((MAP_SIZE * MAP_SIZE)));
        if(map[num] == '_'){
            map[num] = '$';
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

int findSnakeHead(char * map){
    for(int i = 0; i < MAP_SIZE * MAP_SIZE; i++){
        if(map[i] == '^'){
            return i;
        }
    }
    return -1;
}

void step(char * map){
    int snakeLocation = findSnakeHead(map);
    
    if(snakeLocation < 0 && snakeLocation >= MAP_SIZE * MAP_SIZE){
        gameProgress = 'L';
    } else if(currentDirection == 'N'){
        if(snakeLocation - MAP_SIZE >= 0){
            snakeLocation -= MAP_SIZE;
        } else {
            gameProgress = 'L';
        }
    } else if(currentDirection == 'S'){
        if(snakeLocation + MAP_SIZE >= 0){
            snakeLocation += MAP_SIZE;
        } else {
            gameProgress = 'L';
        }
    } else if(currentDirection == 'W'){
        if(snakeLocation%MAP_SIZE - 1 >= 0){
            snakeLocation -= 1;
        } else {
            gameProgress = 'L';
        }
    } else if(currentDirection == 'E'){
        if(snakeLocation%MAP_SIZE + 1 >= MAP_SIZE){
            snakeLocation += 1;
        } else {
            gameProgress = 'L';
        }
    }
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    // Use current time as
    // seed for random generator
    srand(time(0));
    
    char * currentMap = createMap();
    addSnakeHead(currentMap);
    addRandomFood(currentMap);
    renderScreen(currentMap);
    
    return 0;
}
