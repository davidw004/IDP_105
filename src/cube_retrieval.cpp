#include "cube_retrieval.h"

Cube_Retrieval::Cube_Retrieval()
{
    baseSpeed = 250;
    closingSpeed = 250;
    _pickupStart = 0;
    _pickupEnd = 0;
    _releaseTime = 1000;
    _atEnd = false;
}

void Cube_Retrieval::setup()
{
    pinMode(BUTTON1, INPUT);
    //pinMode(BUTTON2, INPUT);
    pinMode(LIMITSWITCH, INPUT);
    pinMode(ULTRASOUND, INPUT);
    _clawMotor = AFMS.getMotor(3);
    AFMS.begin();
}

bool Cube_Retrieval::pickUp() //code to pickup and detect block
{   
    //logic to detect cube and display LEDs
    _blockType = detectCube();
    if (_blockType == HARDBLOCK) digitalWrite(REDLED, HIGH);
    else digitalWrite(GREENLED, HIGH);

    //Pick up block
    raiseClaw();
    return _blockType;
}

/*void Cube_Retrieval::test()
{
    _clawMotor -> setSpeed(baseSpeed);
    if (digitalRead(BUTTON1) == HIGH) {
        _clawMotor -> run(BACKWARD);
    }
    else if (digitalRead(BUTTON2) == HIGH){
        _clawMotor -> run(FORWARD);
     }
    else {
        _clawMotor -> run(RELEASE);
    }
}*/

void Cube_Retrieval::ultrasoundTest()
{
    float reading = analogRead(ULTRASOUND)* MAX_RANG / ADC_SOLUTION;
    Serial.print(reading);
    Serial.print(" ");
    delay(1000);
}

float Cube_Retrieval::readUltrasound()
{
    return analogRead(ULTRASOUND)* MAX_RANG / ADC_SOLUTION;
}

void Cube_Retrieval::raiseClaw() // Closes then raises claw
{   
    _atEnd = false;
    _clawMotor -> setSpeed(baseSpeed);
    while (!_atEnd){
        _clawMotor -> run(BACKWARD);
        if (!digitalRead(LIMITSWITCH))
        {
            _atEnd = true;
            _clawMotor -> run(RELEASE);
        }
    }
}

bool Cube_Retrieval::detectCube() //Can be ran for both commercial and industrial
{
    if (readUltrasound() < 5) return HARDBLOCK;
    else return SOFTBLOCK;
}


void Cube_Retrieval::dropOff() //Drops block into home
{   
    _clawMotor -> setSpeed(baseSpeed);
    _clawMotor -> run(FORWARD);
    delay(2000);
    _clawMotor -> run(RELEASE);
}

void Cube_Retrieval::prepare(uint16_t duration = 0) //Pass in duration, smaller for commercial zone
{   
    if (duration = 0)
    {
        _atEnd = false;
        _clawMotor -> setSpeed(baseSpeed);
        while (!_atEnd){
            _clawMotor -> run(FORWARD);
            if (!digitalRead(LIMITSWITCH2))
            {
                _atEnd = true;
                _clawMotor -> run(RELEASE);
            }
        }
    }
    else 
    {
        _clawMotor -> setSpeed(baseSpeed);
        _clawMotor -> run(FORWARD);
        delay(duration);
        _clawMotor-> run(RELEASE);  
    }
}
/*
    _clawMotor -> setSpeed(baseSpeed);
    _clawMotor -> run(FORWARD);
    delay(duration);
    _clawMotor-> run(RELEASE);
    */   
