#include "Arduino.h"
#include "Line_Follower.h"
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include <SPI.h>

Line_Follower::Line_Follower(uint8_t PWM_R_EN, uint8_t PWM_L_EN, uint8_t MAX_PWM_SPEED)
{
    _PWM_R_EN = PWM_R_EN;
    _PWM_L_EN = PWM_L_EN;
    _MAX_PWM_SPEED = (255 * MAX_PWM_SPEED) / 100;
    baseSpeedRight = (200);
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    pinMode(_PWM_R_EN, OUTPUT);
    pinMode(_PWM_L_EN, OUTPUT);
    // Create the motor shield object with the default I2C address
    Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
    // Select which 'port' M1, M2, M3 or M4. In this case, M1

    AFMS.begin();

}

void Line_Follower::go() {
    //Read input from sensors (not yet figured out what values are being returned yet)
    _A0 = digitalRead(A0);  
    _A1 = digitalRead(A1); //Left Sensor In 
    _A2 = digitalRead(A2); //Right Sensor In
    _A3 = digitalRead(A3);


    Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
    Adafruit_DCMotor *leftMotor = AFMS.getMotor(1);
    // You can also make another motor on port M2
    Adafruit_DCMotor *rightMotor = AFMS.getMotor(2);
    /*if ((_A0 == 1 || _A3 == 1)){
        //call left/right functioin depending on which value in array we are at and //increase array to show a junction has been reached
        junction();
    }*/
    //If both A1 A2 black keep driving at maxspeed
    if (_A1 == 0 && _A2 == 0){
        leftMotor -> setSpeed(150);
        rightMotor -> setSpeed(150);
        //analogWrite(_PWM_L_EN, baseSpeedLeft);
        //analogWrite(_PWM_R_EN, baseSpeedRight);
    }
    else if (_A1 == 1 && _A2 == 0){ //If left high and right low then change motor speed to turn left
        //code to check duration and set weighting for left and right (fraction of speed increase)
        rightMotor -> setSpeed(200);
        leftMotor -> setSpeed(100);
        //analogWrite(_PWM_L_EN, baseSpeedLeft);
        //analogWrite(_PWM_R_EN, baseSpeedRight);
    }
    
    else if (_A1 == 0 && _A2 == 1){ //If left high and right low then change motor speed to turn left
        //code to check duration and set weighting for left and right (fraction of speed increase)
        rightMotor -> setSpeed(200);
        leftMotor -> setSpeed(100);
        /*baseSpeedLeft = _MAX_PWM_SPEED * 0.75 + _left_weight;
        baseSpeedRight = _MAX_PWM_SPEED * 0.75 - _right_weight;
        analogWrite(_PWM_L_EN, baseSpeedLeft);
        analogWrite(_PWM_R_EN, baseSpeedRight);*/
    }

    /*else if (_A1 == 1 && _A2 == 1){
        //reached T junction, initiate turn
        
    }*/
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
    analogWrite(_PWM_R_EN, 0);
    analogWrite(_PWM_L_EN, 0);
}