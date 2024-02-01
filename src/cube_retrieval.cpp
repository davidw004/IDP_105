#include "cube_retrieval.h"

Cube_Retrieval::Cube_Retrieval()
{
    baseSpeed = 250;
    closingSpeed = 200;
}

void Cube_Retrieval::setup(){
    _clawMotor = AFMS.getMotor(3);
    AFMS.begin();
    _clawMotor -> setSpeed(baseSpeed);
    pinMode(BUTTON1, INPUT);
}

void Cube_Retrieval::test(){

    if (digitalRead(BUTTON1) == HIGH) {
        _clawMotor -> run(BACKWARD);
         printf("BUTTON HIGH");
    }
    else if (digitalRead(BUTTON2) == HIGH){
        _clawMotor -> run(FORWARD);
         printf("BUTT LOW");
     }
    else {
        _clawMotor -> run(RELEASE);
    }
}