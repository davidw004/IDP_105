#include "cube_retrieval.h"

Cube_Retrieval::Cube_Retrieval()
{
    baseSpeed = 250;
    startTime = 0;
    closingSpeed = 200;
    _pickupStart = 0;
    _pickupEnd = 0;
    _releaseTime = 1000;
    atEnd = false;
}

void Cube_Retrieval::setup(){
    pinMode(BUTTON1, INPUT);
    pinMode(BUTTON2, INPUT);
    pinMode(LIMITSWITCH, INPUT);
    _clawMotor = AFMS.getMotor(3);
    AFMS.begin(); //Will need to be deleted when using this class within linefollower
}

bool Cube_Retrieval::pickUp() //code to pickup and detect block
{   
    raiseClaw();
    //logic to detect cube
    
    /*if (_pickupEnd - _pickupStart <= 5000) _blockType = HARDBLOCK;
    else _blockType = SOFTBLOCK;*/
    _blockType = HARDBLOCK;

    return _blockType;
}

void Cube_Retrieval::test(){
    Serial.print(" testing");
    _clawMotor -> setSpeed(baseSpeed);
    if (digitalRead(BUTTON1) == HIGH) {
        _clawMotor -> run(BACKWARD);
         Serial.print("BUTTON HIGH");
    }
    else if (digitalRead(BUTTON2) == HIGH){
        _clawMotor -> run(FORWARD);
         Serial.print("BUTT LOW");
     }
    else {
        _clawMotor -> run(RELEASE);
    }
}

void Cube_Retrieval::raiseClaw()
{   
    atEnd = false;
    _clawMotor -> setSpeed(baseSpeed);
    while (!atEnd){
        _clawMotor -> run(BACKWARD);
        if (!digitalRead(LIMITSWITCH)){
            atEnd = true;
            _clawMotor -> run(RELEASE);
        }
    }
}

void Cube_Retrieval::dropOff()
{   
    _clawMotor -> setSpeed(baseSpeed);
    if (_blockType == HARDBLOCK)
    {
        _releaseTime = 5000;
    }
    else if (_blockType = SOFTBLOCK)
    {
        _releaseTime = 7000;
    }
    _clawMotor -> run(FORWARD);
    delay(_releaseTime);
}

void Cube_Retrieval::prepare()
{   
    Serial.print("in function");
    _clawMotor -> setSpeed(baseSpeed);
    _clawMotor -> run(FORWARD);
    delay(5000);
    _clawMotor-> run(RELEASE);   
}