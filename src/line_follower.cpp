#include "Line_Follower.h"
#include "ArduinoPinout.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Line_Follower::Line_Follower()
{
}

void Line_Follower::setup()
{
    //pinMode(LINESENSOR1, INPUT);
    pinMode(LINESENSOR2, INPUT);
    pinMode(LINESENSOR3, INPUT);
    //pinMode(LINESENSOR4, INPUT);

    _leftMotor = AFMS.getMotor(1);
    _rightMotor = AFMS.getMotor(2);
    AFMS.begin();

    _leftMotor->setSpeed(baseSpeedLeft);
    _rightMotor->setSpeed(baseSpeedRight);
    _leftMotor->run(BACKWARD);
    _rightMotor->run(BACKWARD);
}

void Line_Follower::go() {
    //Read input from sensors (not yet figured out what values are being returned yet)
    //_extremeLeftReading = digitalRead(LINESENSOR1);
    Serial.print("Running");
    _leftReading = digitalRead(LINESENSOR2);
    _rightReading = digitalRead(LINESENSOR3);
    //_extremeRightReading = digitalRead(LINESENSOR4);
    
    /*if ((_extremeLeftReading == 1 || _extremeRightReading == 1)){
        //call left/right functioin depending on which value in array we are at and //increase array to show a junction has been reached
        junction();
    }*/
    //If both A1 A2 black keep driving at maxspeed
    if (_leftReading == 0 && _rightReading == 0){
        _leftMotor -> setSpeed(baseSpeedLeft);
        _rightMotor -> setSpeed(baseSpeedRight);
    }
    else if (_leftReading == 1 && _rightReading == 0){ //If left high and right low then change motor speed to turn left
        //code to check duration and set weighting for left and right (fraction of speed increase)
        _leftMotor -> setSpeed(0.6 * baseSpeedRight);
        _rightMotor -> setSpeed(1.4 * baseSpeedRight);
    }
    
    else if (_leftReading == 0 && _rightReading == 1){ //If left high and right low then change motor speed to turn left
        //code to check duration and set weighting for left and right (fraction of speed increase)
        _leftMotor -> setSpeed(1.4 * baseSpeedRight);
        _rightMotor -> setSpeed(0.6 * baseSpeedRight);
    }
}

/*void Line_Follower::junction(){
    if (currentRoute[pos] == LEFT) //Current syntax needs updating but i think this should work
    { 
            pos+=1;
    }

    else if (currentRoute[pos] == RIGHT)
    { 
        //
    }

    else if (currentRoute[pos] == STRAIGHT)
    {
        //Wait certain duration while turn occurs to prevent double detection
    }

    //If reached end of array, call pick up / detect cube function
    //Cube_Retrieval::pickupCube{};
}
*/

void Line_Follower::stop() {
    _rightMotor -> setSpeed(0);
    _leftMotor -> setSpeed(0);
}