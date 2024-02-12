#include "route_planning.h"

namespace Routes {
    // Block 1 collection
    Decision CollectBlockOne[3] = {LEFT, RIGHT, BLOCK};
    //Return routes 1
    Decision BringBockOneToGreen[3] = {RIGHT, LEFT, HOME};
    Decision BringBockOneToRed[4] = {LEFT, STRAIGHT, RIGHT, HOME};

    // Block 2
    Decision CollectBlockTwoFromRed[4] = {STRAIGHT, LEFT, LEFT, BLOCK};
    Decision CollectBlockTwoFromGreen[5] = {STRAIGHT, RIGHT, STRAIGHT, RIGHT, BLOCK};
    // Return routes 2
    Decision BringBlockTwoToGreen[5] = {LEFT, STRAIGHT, LEFT, STRAIGHT, HOME};
    Decision BringBlockTwoToRed[4] = {RIGHT, RIGHT, STRAIGHT, HOME};

    // Block Swan
    Decision CollectBlockSFromRed[3] = {STRAIGHT, STRAIGHT, ENTERSWAN};
    Decision CollectBlockSFromGreen[5] = {STRAIGHT, RIGHT, LEFT, RIGHT, ENTERSWAN};
    // Return Swan
    Decision BringBlockSToGreen[] = {EXITSWAN, LEFT, RIGHT, LEFT, STRAIGHT, HOME};
    Decision BringBlockSToRed[] = {EXITSWAN, LEFT, LEFT, STRAIGHT, RIGHT, STRAIGHT, HOME};

    // Block Lucozade
    Decision CollectBlockLFromRed[5] = {STRAIGHT, LEFT, STRAIGHT, RIGHT, ENTERLUCOZADE};
    Decision CollectBlockLFromGreen[4] = {STRAIGHT, RIGHT, LEFT, ENTERLUCOZADE};
    // Return Lucozade
    Decision ReturnBlockLToGreen[] = {EXITLUCOZADE, RIGHT, LEFT, STRAIGHT, HOME};
    Decision ReturnBlockLToRed[] = {EXITLUCOZADE, LEFT, STRAIGHT, RIGHT, STRAIGHT, HOME};
};