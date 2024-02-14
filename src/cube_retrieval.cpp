#include "cube_retrieval.h"

 //Define all member variables
Cube_Retrieval::Cube_Retrieval()
{
    baseSpeed = 250;
    closingSpeed = 250;
    _pickupStart = 0;
    _pickupEnd = 0;
    _releaseTime = 1000;
    _atEnd = false;
}

//Setup pins as I/O ports and setup motors
void Cube_Retrieval::setup()
{
    pinMode(LIMITSWITCH, INPUT);
    pinMode(ULTRASOUND, INPUT);
    _clawMotor = AFMS.getMotor(3);
    AFMS.begin();
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

//Testing ultrasound
void Cube_Retrieval::ultrasoundTest()
{
    float reading = analogRead(ULTRASOUND)* MAX_RANG / ADC_SOLUTION;
    Serial.print(reading);
    Serial.print(" ");
    delay(1000);
}

//Simple ultrasound read
float Cube_Retrieval::readUltrasound()
{
    return analogRead(ULTRASOUND)* MAX_RANG / ADC_SOLUTION;
}

// Closes then raises claw
void Cube_Retrieval::raiseClaw()
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

//Close claw for set duration
void Cube_Retrieval::closeClaw(int duration)
{
    _clawMotor -> setSpeed(baseSpeed);
    _clawMotor -> run(BACKWARD);
    delay(duration);
    _clawMotor -> run(RELEASE);
}

//Detect cube type
bool Cube_Retrieval::detectCube() 
{
    if (readUltrasound() < 6) {digitalWrite(REDLED, HIGH); return HARDBLOCK;}
    else {digitalWrite(GREENLED, HIGH); return SOFTBLOCK;}
}

//Drop cube into home section
void Cube_Retrieval::dropOff()
{   
    _clawMotor -> setSpeed(baseSpeed);
    _clawMotor -> run(FORWARD);
    delay(4000);
    _clawMotor -> run(RELEASE);
}

//Open claw in preparation to pickup cube
void Cube_Retrieval::prepare() 
{   
    Serial.print("in prepare func ");
    /*if (duration == 0)
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
    }*/
    _clawMotor -> setSpeed(baseSpeed);
    _clawMotor -> run(FORWARD);
    delay(7500);
    _clawMotor-> run(RELEASE);  
}
