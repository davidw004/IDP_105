#include "route_planning.h"

namespace Routes {
    // Block 1
    Decision routeOne[3] = {LEFT, RIGHT, BLOCK};
    //Return routes 1
    Decision returnOneGreen[3] = {RIGHT, LEFT, HOME};
    Decision returnOneRed[4] = {LEFT, STRAIGHT, RIGHT, HOME};

    // Block 2
    Decision routeTwoRed[4] = {STRAIGHT, LEFT, LEFT, BLOCK};
    Decision routeTwoGreen[5] = {STRAIGHT, RIGHT, STRAIGHT, RIGHT, BLOCK};
    //Return routes 2
    Decision returnTwoGreen[5] = {LEFT, STRAIGHT, LEFT, STRAIGHT, HOME};
    Decision returnTwoRed[4] = {RIGHT, RIGHT, STRAIGHT, HOME};
};