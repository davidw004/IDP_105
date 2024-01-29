#include "Line_Follower.h"

#include "ArduinoPinout.h"

Line_Follower::Line_Follower()
{
    //pinMode(LINESENSOR1, INPUT);
    pinMode(LINESENSOR2, INPUT);
    pinMode(LINESENSOR3, INPUT);
    //pinMode(LINESENSOR4, INPUT);

    // Create the motor shield object with the default I2C addres
    Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
    Adafruit_DCMotor *leftMotor = AFMS.getMotor(1);
    // You can also make another motor on port M2
    Adafruit_DCMotor *rightMotor = AFMS.getMotor(2);
    // Select which 'port' M1, M2, M3 or M4. In this case, M1
    AFMS.begin();
    leftMotor->setSpeed(baseSpeedLeft);
    rightMotor->setSpeed(baseSpeedRight);
}

void Line_Follower::go() {
    //Read input from sensors (not yet figured out what values are being returned yet)
    //_extremeLeftReading = digitalRead(LINESENSOR1);
    _leftReading = digitalRead(LINESENSOR2);
    _rightReading = digitalRead(LINESENSOR3);
    //_extremeRightReading = digitalRead(LINESENSOR4);
    
    leftMotor->run(FORWARD);
    rightMotor->run(FORWARD);
    
    /*if ((_extremeLeftReading == 1 || _extremeRightReading == 1)){
        //call left/right functioin depending on which value in array we are at and //increase array to show a junction has been reached
        junction();
    }*/
    //If both A1 A2 black keep driving at maxspeed
    if (_leftReading == 0 && _rightReading == 0){
        leftMotor -> setSpeed(baseSpeedLeft);
        rightMotor -> setSpeed(baseSpeedRight);
    }
    else if (_leftReading == 1 && _rightReading == 0){ //If left high and right low then change motor speed to turn left
        //code to check duration and set weighting for left and right (fraction of speed increase)
        leftMotor -> setSpeed(0.8 * baseSpeedRight);
        rightMotor -> setSpeed(1.2 * baseSpeedRight);
    }
    
    else if (_leftReading == 0 && _rightReading == 1){ //If left high and right low then change motor speed to turn left
        //code to check duration and set weighting for left and right (fraction of speed increase)
        leftMotor -> setSpeed(1.2 * baseSpeedRight);
        rightMotor -> setSpeed(0.8 * baseSpeedRight);
    }
}

void Line_Follower::junction(){
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

void Line_Follower::stop() {
    rightMotor -> setSpeed(0);
    leftMotor -> setSpeed(0);
}