#include "Line_Follower.h"
#include "ArduinoPinout.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Line_Follower::Line_Follower()
{
    maxSpeedLeft = 200;
    maxSpeedRight = 200;
    baseSpeedLeft = 150;
    baseSpeedRight = 150;
    blocksCollected = 0;
    pos = 0;
    _currentRoute = Routes::routeOne;
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
}

void Line_Follower::exitbox(){
    //Position box physically in the middle of start
    
    //Run motors forwards
    _leftMotor->run(FORWARD); //motors are connected in reverse
    _rightMotor->run(FORWARD); //motors are connected in reverse

    //Wait until exited box, ie central line sensors are past the white line
    delay(1000); //Fine tune duration
    _leftMotor->run(RELEASE);
    _rightMotor->run(RELEASE); 

}

void Line_Follower::go() {
    Serial.print("Running");

    //Read input from sensors (not yet figured out what values are being returned yet)
    //_extremeLeftReading = digitalRead(LINESENSOR1);
    _leftReading = digitalRead(LINESENSOR2);
    _rightReading = digitalRead(LINESENSOR3);
    //_extremeRightReading = digitalRead(LINESENSOR4);
    
    //Run motors forwards
    _leftMotor->run(FORWARD); //motors are connected in reverse
    _rightMotor->run(FORWARD); //motors are connected in reverse

    //if ((_extremeLeftReading == 1 || _extremeRightReading == 1)){
    //    junction();
    //}

    //If both middle sensors black keep driving at maxspeed
    if (_leftReading == 0 && _rightReading == 0){
        _leftMotor -> setSpeed(maxSpeedLeft);
        _rightMotor -> setSpeed(maxSpeedRight);
    }
    else if (_leftReading == 1 && _rightReading == 0){ //If left high and right low then change motor speed to turn left
        _leftMotor -> setSpeed(0);
        _rightMotor -> setSpeed(1.4 * baseSpeedRight);
    }
    
    else if (_leftReading == 0 && _rightReading == 1){ //If left high and right low then change motor speed to turn left
        _leftMotor -> setSpeed(1.4 * baseSpeedLeft);
        _rightMotor -> setSpeed(0);
    }
}


void Line_Follower::junction(){
    /*switch (_currentRoute[pos]){  
        case LEFT:
        {
            //Turn left code
            _leftMotor -> setSpeed(baseSpeedLeft);
            _rightMotor -> setSpeed(baseSpeedRight);
            _leftMotor->run(BACKWARD);  // Replace if needed
            _rightMotor->run(FORWARD);  // Replace if needed
            //wait duration of time
            delay(1000);
            pos++;
            break;
        }
        case RIGHT:
        {
            //Turn right code
            _leftMotor -> setSpeed(baseSpeedLeft);
            _rightMotor -> setSpeed(baseSpeedRight);
            _leftMotor->run(BACKWARD);  // Replace if needed
            _rightMotor->run(FORWARD);
            //wait duration of time
            delay(1000);
            pos++;
            break;
        }
        case STRAIGHT:
        {
            //Continue straight (prevent double detection)
            _leftMotor -> setSpeed(baseSpeedLeft);
            _rightMotor -> setSpeed(baseSpeedRight);
            _leftMotor->run(BACKWARD);  // Replace if needed
            _rightMotor->run(BACKWARD);
            delay(1000);
            pos++;
            break;
        }
        default:{
            break;
        }
    }
        switch(_currentRoute[pos]){
        case BLOCK:
        {
            //Collect block
            //Detect block and select route home
            //next array
            blocksCollected++;
            //If block soft and blocks selected == 1,2 etc select route home
            break;
        }
        case HOME:
        {
            //Drop off cube
            //next array
            //If blocksCollected = 1,2 etc select route to next block
            //Spin 180 to begin next path
            break;
        }
        default:{
            break;
        }
    }*/
}

void Line_Follower::stop() {
    _rightMotor -> setSpeed(0);
    _leftMotor -> setSpeed(0);
}