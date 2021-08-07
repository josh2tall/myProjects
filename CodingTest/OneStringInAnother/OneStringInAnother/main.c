//
//  C programming language used
//  main.c
//  OneStringInAnother
//
//  Created by Joshua Acosta.
//

#include <stdio.h>
#include <stdbool.h>

//Determines if string1 is in string2. Iterates through string1 and string2 until a match is found.
//string2's index always iterates by 1 but string1's index will only iterate if a potential match is found.
//string1's index resets back to 0 if only a partial match is found. This allows the next potential match can be found.
//@param string1 - the first string
//@param string2 - the second string
//return true if string1 is in string2, otherwise false
bool isStringInOtherString(char * string1, char * string2);

int main(int argc, const char * argv[]) {
    char * string1 = "fox";
    char * string2 = "thisisfox";
    
    bool result = isStringInOtherString(string1, string2);
    
    if(result)
    {
        printf("string1 is in string2");
    }
    else
    {
        printf("string1 is not in string2");
    }
    
    return 0;
}

bool isStringInOtherString(char * string1, char * string2)
{
    unsigned int index1 = 0;
    unsigned int index2 = 0;
    bool string1IsPresentInString2 = false;
    bool isMatchInProgress = false;
    bool done = false;
    
    while(!done)
    {
        if(string1[index1] == string2[index2])
        {
            index1++;
            index2++;
            isMatchInProgress = true;
            
            //A match has been found
            if(string1[index1] == '\0')
            {
                string1IsPresentInString2 = true;
            }
        }
        else
        {
            //Only a partial match was found so far
            if(isMatchInProgress)
            {
                isMatchInProgress = false;
                index1 = 0;
            }
            
            index2++;
        }
        
        if(string1[index1] == '\0' || string2[index2] == '\0')
        {
            done = true;
        }
    }
    
    return string1IsPresentInString2;
}
