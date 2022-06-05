//
//  main.cpp
//  LifeTrack
//
//  Created by Joshua Acosta on 1/19/18.
//  Copyright © 2018 Joshua Acosta. All rights reserved.
//

#include <iostream>
#include "LifeTrack.h"



int main() {
    bool exit = false;
    
    while(!exit)
    {
        cout << ">";
        exit = initial();
    }
    return 0;
}
