#include "route_planning.h"

namespace Routes {

    // Block 1 collection
    Decision CollectBlockOne[3] = {LEFT, RIGHT, BLOCK};
    
    //Return routes 1
    Decision BringBlockOneToGreen[3] = {RIGHT, LEFT, HOME};
    Decision BringBlockOneToRed[4] = {LEFT, STRAIGHT, RIGHT, HOME};

    // Block 2
    Decision CollectBlockTwoFromRed[4] = {STRAIGHT, LEFT, LEFT, BLOCK};
    Decision CollectBlockTwoFromGreen[5] = {STRAIGHT, RIGHT, STRAIGHT, RIGHT, BLOCK};
    
    // Return routes 2
    Decision BringBlockTwoToGreen[5] = {LEFT, STRAIGHT, LEFT, STRAIGHT, HOME};
    Decision BringBlockTwoToRed[4] = {RIGHT, RIGHT, STRAIGHT, HOME};


    // Block Swan
    Decision CollectBlockSFromRed[6] = {STRAIGHT, LEFT, STRAIGHT, RIGHT, RIGHT, ENTERSWAN};
    Decision CollectBlockSFromGreen[5] = {STRAIGHT, RIGHT, LEFT, RIGHT, ENTERSWAN};
    // Return Swan
    Decision BringBlockSToGreen[5] = {LEFT, RIGHT, LEFT, STRAIGHT, HOME};
    Decision BringBlockSToRed[3] = {STRAIGHT, STRAIGHT, HOME};

    // Block Lucozade
    Decision CollectBlockLFromRed[6] = {STRAIGHT, LEFT, STRAIGHT, STRAIGHT, RIGHT, ENTERLUCOZADE};
    Decision CollectBlockLFromGreen[4] = {STRAIGHT, STRAIGHT, RIGHT, ENTERLUCOZADE};
    // Return Lucozade
    Decision BringBlockLToGreen[4] = {RIGHT, LEFT, STRAIGHT, HOME};
    Decision BringBlockLToRed[5] = {LEFT, STRAIGHT, RIGHT, STRAIGHT, HOME};

    //Back to home boxes
    Decision ReturnHomeFromRed[3] = {LEFT, LEFT, STARTBOX};
    Decision ReturnHomeFromGreen[3] = {RIGHT, RIGHT, STARTBOX};
};