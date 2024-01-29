#pragma once
#include "Arduino.h"

enum Decision{STRAIGHT, LEFT, RIGHT, NULL_DECISION};

/*
/struct Route {
    Decision* decisions;  // Adjust the size based on your maximum expected decisions
    size_t numDecisions;
};
*/
/*
namespace Routes {
    // Block 1
    Decision routeOne[MAX_DECISIONS] = {LEFT, RIGHT, NULL_DECISION, NULL_DECISION};
    Decision returnOneGreen[MAX_DECISIONS] = {RIGHT, LEFT, NULL_DECISION, NULL_DECISION};
    Decision returnOneRed[MAX_DECISIONS] = {LEFT, STRAIGHT, RIGHT, NULL_DECISION};

    // Block 2
    Decision routeTwoRed[MAX_DECISIONS] = {STRAIGHT, LEFT, LEFT, NULL_DECISION};
    Decision routeTwoGreen[MAX_DECISIONS] = {STRAIGHT, RIGHT, STRAIGHT, RIGHT};
    Decision returnTwoGreen[MAX_DECISIONS] = {LEFT, STRAIGHT, LEFT, STRAIGHT};
    Decision returnTwoRed[MAX_DECISIONS] = {RIGHT, RIGHT, STRAIGHT, NULL_DECISION};
};
*/
//malloc
//free

// identify max length
// decisions to an array of max length, fill remaining gaps with NULL

