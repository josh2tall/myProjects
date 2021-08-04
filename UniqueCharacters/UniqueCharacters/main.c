//
//  main.c
//  UniqueCharacters
//
//  Checks if all the characters in the "test" string are unique
//
//  Created by Joshua Acosta on 8/3/21.
//

#include <stdio.h>
#include <stdlib.h> //for malloc
#include <stdint.h>
#include <stdbool.h>

bool hasAllUniqueCharacters(char * stringToCheck, uint32_t stringSize);

uint32_t getIndex(char characterToCheck);

uint32_t getSizeOfString(char * stringToGetSizeOf);

int main(int argc, const char * argv[]) {
    // insert code here...
    
    char * test = "this work";
    
    uint32_t size = getSizeOfString(test);
    printf("The size is %d \r\n", size);
    bool result = hasAllUniqueCharacters(test, size);
    
    if(result)
    {
        printf("the string has all unique characters\r\n");
    }
    else
    {
        printf("the string does not have all unique characters\r\n");
    }
    
    return 0;
}

bool hasAllUniqueCharacters(char * stringToCheck, uint32_t stringSize)
{
    uint32_t * hashTable = malloc(stringSize * UINT8_MAX);
    bool allCharactersUnique = true;
    
    for(int i = 0; i < UINT8_MAX; i++)
    {
        hashTable[i] = UINT32_MAX;
    }
    
    for(uint32_t i = 0; i < stringSize; i++)
    {
        uint32_t index = stringToCheck[i];
    
        if(hashTable[index] == UINT32_MAX)
        {
            hashTable[index] = stringToCheck[i];
        }
        else
        {
            allCharactersUnique = false;
        }
    }
    return allCharactersUnique; //temporary
}

uint32_t getSizeOfString(char * stringToGetSizeOf)
{
    uint32_t size = 0;
    bool done = false;
    uint32_t iterator = 0;
    while(!done)
    {
        if(stringToGetSizeOf[iterator] != '\0')
        {
            size++;
            iterator++;
        }
        else
        {
            done = true;
        }
    }
    
    return size;
}
