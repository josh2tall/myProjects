//
//  main.c
//  stringPermutationCheck
//
//  Checks to see if two strings are a permutation of one another
//
//  Created by Joshua Acosta on 8/3/21.
//

#include <stdio.h>
#include <stdbool.h>

uint32_t getSizeOfString(char * stringToGetSizeOf);

char * merge_sort(char * stringToSort);

int main(int argc, const char * argv[]) {
    // insert code here...
    char * firstString = "how is it going";
    char * secondString = "woh si ti gonig";
    
    uint32_t size_firstString = getSizeOfString(firstString);
    uint32_t size_secondString = getSizeOfString(secondString);
    
    printf("num characters in string is: %d and %d", size_firstString, size_secondString);
    
    char * sortedFirstString = merge_sort(firstString);
    
    return 0;
}


uint32_t getSizeOfString(char * stringToGetSizeOf)
{
    uint32_t size = 0;
    uint32_t iterator = 0;
    bool done = false;
    
    while(!done)
    {
        if(stringToGetSizeOf[iterator] == '\0')
        {
            done = true;
        }
        else
        {
            size++;
            iterator++;
        }
    }
    
    return size;
}

char * merge_sort(char * stringToSort)
{
    
}
