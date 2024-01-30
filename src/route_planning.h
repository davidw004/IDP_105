#pragma once
#include "Arduino.h"

enum Decision{STRAIGHT, LEFT, RIGHT, BLOCK, HOME};

namespace Routes {
    // Block 1
    extern Decision routeOne[3];
    //Return routes 1
    extern Decision returnOneGreen[3];
    extern Decision returnOneRed[4];

    // Block 2
    extern Decision routeTwoRed[4];
    extern Decision routeTwoGreen[5];
    //Return routes 2
    extern Decision returnTwoGreen[5];
    extern Decision returnTwoRed[4];
};
//malloc
//free

// identify max length
// decisions to an array of max length, fill remaining gaps with NULL