#pragma once
#include "Arduino.h"

enum Decision{STRAIGHT, LEFT, RIGHT};
struct Routes{
    //Block 1
    Decision routeOne[2] = {LEFT,RIGHT};
    Decision returnOneGreen[2] = {RIGHT, LEFT};
    Decision returnOneRed[3] = {LEFT, STRAIGHT, RIGHT};

    //Block 2
    Decision routeTwoRed[3] = {STRAIGHT, LEFT, LEFT};
    Decision routeTwoGreen[4] = {STRAIGHT, RIGHT, STRAIGHT, RIGHT};
    Decision returnTwoGreen[4] = {LEFT, STRAIGHT, LEFT, STRAIGHT};
    Decision returnTwoRed[3] = {RIGHT, RIGHT, STRAIGHT};

    
};