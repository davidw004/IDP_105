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
    leftMotor->setSpeed(150);
    rightMotor->setSpeed(150);
    leftMotor->run(FORWARD);
    rightMotor->run(FORWARD);
}

void Line_Follower::go() {
    //Read input from sensors (not yet figured out what values are being returned yet)
    //_A0 = digitalRead(LINESENSOR1);
    _A1 = digitalRead(LINESENSOR2);
    _A2 = digitalRead(LINESENSOR3);
    //_A3 = digitalRead(LINESENSOR4);

    /*if ((_A0 == 1 || _A3 == 1)){
        //call left/right functioin depending on which value in array we are at and //increase array to show a junction has been reached
        junction();
    }*/
    //If both A1 A2 black keep driving at maxspeed
    if (_A1 == 0 && _A2 == 0){
        leftMotor -> setSpeed(150);
        rightMotor -> setSpeed(150);
    }
    else if (_A1 == 1 && _A2 == 0){ //If left high and right low then change motor speed to turn left
        //code to check duration and set weighting for left and right (fraction of speed increase)
        rightMotor -> setSpeed(200);
        leftMotor -> setSpeed(100);
    }
    
    else if (_A1 == 0 && _A2 == 1){ //If left high and right low then change motor speed to turn left
        //code to check duration and set weighting for left and right (fraction of speed increase)
        rightMotor -> setSpeed(200);
        leftMotor -> setSpeed(100);
    }
}

void Line_Follower::junction(){
    /*if (this->array = left turn) {
        //some function to determine baseSpeed
        also stay in code until turtn complete so while loop??
        analogWrite(_PWM_L_EN, 0);
        analogWrite(_PWM_R_EN, baseSpeedRight);

    else if (this->array = right turn) {
        //some function to determine baseSpeed
        analogWrite(_PWM_L_EN, baseSpeedLeft);
        analogWrite(_PWM_R_EN, 0);

        else if (this->array = straightforward) {
        //some function to determine baseSpeed
        while (_A0 == 1 || _A4 == 1)
        analogWrite(_PWM_L_EN, baseSpeedLeft );
        analogWrite(_PWM_R_EN, baseSpeedRight);
    }*/
    

    //do not return to code unless passed turn for straight line 
}

void Line_Follower::stop() {
    rightMotor -> setSpeed(0);
    leftMotor -> setSpeed(0);
}