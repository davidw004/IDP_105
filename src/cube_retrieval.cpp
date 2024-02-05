#include "cube_retrieval.h"

Cube_Retrieval::Cube_Retrieval()
{
    atEnd = false;
}

void Cube_Retrieval::setup(){
    _clawMotor = AFMS.getMotor(3);
    pinMode(BUTTON1, INPUT);
    pinMode(BUTTON2, INPUT);
    pinMode(BUTTON3, INPUT);
}


bool Cube_Retrieval::pickUp() //code to pickup and detect block
{
    //Pick up block then return 1 if block hard
    _clawMotor -> setSpeed(baseSpeed);
    _pickupStart = millis();
    while (!atEnd){
        _clawMotor -> run(BACKWARD);
        if (digitalRead(BUTTON3)){
            _pickupEnd = millis();
            atEnd = true;
            _clawMotor -> run(RELEASE);
        }
    }
    
    if (_pickupEnd - _pickupStart <= 5000) _blockType = true;
    else _blockType = false;

    return _blockType;
}

void Cube_Retrieval::test(){
    _clawMotor -> setSpeed(baseSpeed);
    if (digitalRead(BUTTON1) == HIGH) {
        _clawMotor -> run(BACKWARD);
         printf("BUTTON HIGH");
    }
    else if (digitalRead(BUTTON2) == HIGH){
        _clawMotor -> run(FORWARD);
         printf("BUTTon LOW");
     }
    else {
        _clawMotor -> run(RELEASE);
    }
}

void Cube_Retrieval::approachCube(&Line_Follower LF)
{
    uint32_t startTime = millis();
    auto distFromWall = 
    while ((millis() - startTime) < 3000)
    {
        LF.go(); //Will need fine tuning
    }
}

void Cube_Retrieval::dropOff()
{
    _clawMotor -> setSpeed(baseSpeed);
    _clawMotor -> run(FORWARD);
}
