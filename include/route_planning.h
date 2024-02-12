#pragma once

enum Decision{STRAIGHT, LEFT, RIGHT, BLOCK, HOME, ENTERSWAN, ENTERLUCOZADE, EXITSWAN, EXITLUCOZADE, STARTBOX};

namespace Routes {

    // Block 1
    extern Decision CollectBlockOne[3];

    //Return routes 1
    extern Decision BringBlockOneToGreen[3];
    extern Decision BringBlockOneToRed[4];

    // Block 2
    extern Decision CollectBlockTwoFromRed[4];
    extern Decision CollectBlockTwoFromGreen[5];
    
    //Return routes 2
    extern Decision BringBlockTwoToGreen[5];
    extern Decision BringBlockTwoToRed[4];

    // Block 2
    extern Decision CollectBlockSFromRed[6];
    extern Decision CollectBlockSFromGreen[5];
    //Return routes 2
    extern Decision BringBlockSToGreen[5];
    extern Decision BringBlockSToRed[3];

    extern Decision CollectBlockLFromRed[6];
    extern Decision CollectBlockLFromGreen[4];
    // Return Lucozade
    extern Decision BringBlockLToGreen[4];
    extern Decision BringBlockLToRed[5];

    //Back to home boxes
    extern Decision ReturnHomeFromRed[3];
    extern Decision ReturnHomeFromGreen[3];

};
//malloc
//free

// identify max length
// decisions to an array of max length, fill remaining gaps with NULL