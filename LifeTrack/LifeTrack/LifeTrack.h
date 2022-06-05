//
//  LifeTrack.h
//  LifeTrack
//
//  Created by Joshua Acosta on 1/19/18.
//  Copyright © 2018 Joshua Acosta. All rights reserved.
//

#ifndef LifeTrack_h
#define LifeTrack_h

#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <fstream>
using namespace std;


//returns a line from cin
string readInput()
{
    string input;
    
    getline(cin,input);
    
    return input;
}

void lineToLower(string& input)
{
    for(int i = 0; i < input.length(); i++)
    {
        tolower(input[i]);
    }
}

void workout()
{
    cout << "would you like to add, delete, or view a workout?";

    string input = readInput();
    lineToLower(input);
    string date;
    bool cont = true;
    
    if(input == "add")
    {
        cout << "what is the date mm/dd/yyyy?:" << endl;
        
        date = readInput();
        
    }
    else
    {
        cout << "not adding workout" << endl;
    }
    
}

void food()
{
    string input = readInput();
    lineToLower(input);
    
    if(input == "add")
    {
        cout << "adding food" << endl;
    }
    else
    {
        cout << "not adding food" << endl;
    }
}

void readFile()
{
    bool open = false;
    string data;
    fstream filename;

    
    while(!open)
    {
        cout << "what file would you like to read?" << endl;
        
        string input = readInput();
        
        filename.open(input);
        
        if(filename.is_open()) {open = true;}
        else { cout << "file could not be opened" << endl;}
    }
    
    while(!filename.eof())
    {
        getline(filename, data);
        cout << data << endl;
        data = "";
    }
    
    filename.close();
}

void createFile()
{
    string data;
    ofstream outputFile;
    
    cout << "what would you like the file to be named?: " << endl;
    string input = readInput();
    
    outputFile.open(input);
    
    cout << "The file " << input << " was created" << endl;
    
    outputFile.close();
}

void writeToFile()
{
    ofstream outputFile;
    
    cout << "What file would you like to write to?: " << endl;
    
    string filename = readInput();
    
    outputFile.open(filename);
    
    if(!outputFile.is_open())
    {
        cout << "file was not found" << endl;
    }
    else
    {
        cout << "what would you like to write to the file?"<< endl;
        
        string data = readInput();
        
        outputFile << data;
    }
    
    outputFile.close();
}

bool initial()
{
    string input = readInput();
    lineToLower(input);
    
    if(input == "workout")
    {
        workout();
    }
    else if(input == "food")
    {
        food();
    }
    else if(input == "readfile")
    {
        readFile();
    }
    else if(input == "createfile")
    {
        createFile();
    }
    else if(input == "writefile")
    {
        writeToFile();
    }
    else if(input == "exit")
    {
        return true;
    }

    else
    {
        cout << "not a valid command" << endl;

    }
    
    return false;
}







#endif /* LifeTrack_h */
