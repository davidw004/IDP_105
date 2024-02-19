# Overview

This folder contains all the user written header files, including all hardware setup function declarations, both class declarations (within cuve_retrieval.h and line_follower.h), the route plan structure and the pins used for connecting to the arduino board.

## Line Follower

Within the Line follower header file, all driving related functions are declared and member variables are also declared (defined in the setup function). The two motors are also only declared within this class so as to limit their usage to be exclusively within the class.

The Line_Follower class also encorporates composition, utilising the Cube_Retrieval and TOFSensor classes in order to establish a modular and reusable structure and promoting code organisation. Many of the member functions within the Line_Follower could have been split up into separate classes (e.g reading line sensors and running motors) however as the project was relatively short there was not enough time for perfect code organisation. 

## Cube Retrieval

The Cube Retrieval class is structured similarly to the Line Follower class with a setup function and multiple member functions and variables. Many of the functions utilised delays to time functions (opening/closing the claw to a given position).


## Route Planning

This file declared the decions to be made at junctions: {STRAIGHT, LEFT, RIGHT, BLOCK, HOME, ENTERSWAN, ENTERLUCOZADE, EXITSWAN, EXITLUCOZADE, STARTBOX}; and declared all the routes used in the competition.
