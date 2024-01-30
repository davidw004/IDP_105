#include "cube_retrieval.h"
#include "ArduinoPinout.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Cube_Retrieval::Cube_Retrieval()
{
}

void Cube_Retrieval::setup(){


}
void Cube_Retrieval::collect(){
    _clawMotor = AFMS.getMotor(3);
    delay 2000;
    _clawMotor -> setSpeed(closingSpeed);
}
void Cube_Retrieval::fetch1(){
    /* go forward specifed distance by running line 
    follower code for a set amount of time and calling void. 
    Run written block collection code (written in this class)
    before running line follower reverse code(needs to be 
    written)*/
}
