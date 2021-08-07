//
//  C programming language used
//  main.c
//  RepresentAnArray
//
//  Created by Joshua Acosta.
//

#include <stdio.h>
#include <stdlib.h>

const unsigned int SIZE_OF_ARRAY = 5;
unsigned int * MyArray = NULL;
unsigned int numRotations = 0;

//Converts the index to another that uses numRotations
//@param index - the index in the psuedo array that the user wants
//@return the real index in the psuedo array that holds the desired element
unsigned int FindIndex(unsigned int index);

//Retrieves the element at a certain index.
//@param index - the location in the array
//@return the value of the element at the index passed in
unsigned int GetAt(unsigned int index);

//Increments the numRotations variable that represents how many times the array was rotated
void Rotate(unsigned int num);

//Prints MyArray
void PrintMyArray(void);

int main(int argc, const char * argv[])
{
    //Allocate continguous memory
    MyArray = malloc(SIZE_OF_ARRAY * sizeof(unsigned int));
    
    //fill MyArray with an incrementing pattern
    for(unsigned int i = 0; i < SIZE_OF_ARRAY; i++)
    {
        *(MyArray + i) = i;
    }
    
    PrintMyArray();
    
    Rotate(1);
    
    PrintMyArray();

    printf("element at index 1: %u \r\n", GetAt(1));
    
    Rotate(3);

    PrintMyArray();
    
    printf("element at index 1: %u \r\n", GetAt(1));

    return 0;
}

unsigned int FindIndex(unsigned int index)
{
    return (index + numRotations) % SIZE_OF_ARRAY;
}

unsigned int GetAt(unsigned int index)
{
    return *(MyArray + FindIndex(index));
}

void Rotate(unsigned int num)
{
    numRotations += num;
}
           
void PrintMyArray(void)
{
    for(unsigned int i = 0; i < SIZE_OF_ARRAY; i++)
    {
        printf("%u ", GetAt(i));
    }

    printf("\r\n");
}
