//
//  C programming language used
//  main.c
//  CodingTest
//
// Convert an integer from decimal to binary
//
//  Created by Joshua Acosta.
//

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define NUM_BITS_IN_UNSIGNED_INT 32U

// Converts a number from a decimal representation to a binary representation
// @param numToConvert - The decimal number that requires conversion to binary
// @param binRepresentation - a pointer to where to binary representation is stored
// @return the most significant bit's index
unsigned int ConvertFromDecToBin(unsigned int numToConvert, char binRepresentation[]);

//Prints the binary representation of an integer
//@param binRepresentation - character array that holds the number
//@param MSBLocation - index where the binary numbebr starts
void PrintBinString(char binRepresentation[], unsigned int MSBIndex);


int main(int argc, const char * argv[]) {
    unsigned int needsConversion = 8;
    char binRepresentation[NUM_BITS_IN_UNSIGNED_INT];
    
    for(int i = 0; i < NUM_BITS_IN_UNSIGNED_INT; i++)
    {
        binRepresentation[i] = '\0';
    }
    
    unsigned int MSBIndex = ConvertFromDecToBin(needsConversion, binRepresentation);

    printf("the binary representation of %d is: ", needsConversion);
    PrintBinString(binRepresentation, MSBIndex);
    
    return 0;
}

unsigned int ConvertFromDecToBin(unsigned int numToConvert, char binRepresentation[])
{
    unsigned int conversionInProgress = numToConvert;
    unsigned int currentIndex = 0;
    
    while(conversionInProgress != 0)
    {
        binRepresentation[NUM_BITS_IN_UNSIGNED_INT - currentIndex - 1] = (char) (conversionInProgress % 2);
        conversionInProgress /= 2;
        currentIndex++;
    }
    
    return currentIndex;
}

void PrintBinString(char binRepresentation[], unsigned int MSBIndex)
{
    printf("0b");
    for(unsigned int i = NUM_BITS_IN_UNSIGNED_INT - MSBIndex; i < NUM_BITS_IN_UNSIGNED_INT; i++)
    {
        printf("%u", binRepresentation[i]);
    }
    
    printf("\r\n");
}
