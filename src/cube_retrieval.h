#pragma once
#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include <SPI.h>

class Cube_Retrieval 
{
    public:
        Cube_Retrieval();
        void setup();
        void collect();
        void fetch1();
    //Constructor
    //Function for moving forward certain distance up to cube
    //Function for detecting type of cube and turning on light
    //Function for picking up cube

    private:
        uint8_t _extremeLeftReading, _leftReading, _rightReading, _extremeRightReading;
        uint8_t _left_weight;
        uint8_t _right_weight;
        uint8_t maxSpeedLeft = 200;
        uint8_t maxSpeedRight = 200;
        uint8_t baseSpeedLeft = 150;
        uint8_t baseSpeedRight = 150;
        uint8_t blocksCollected = 100;
        uint8_t closingSpeed = 200;
        Adafruit_DCMotor *_clawMotor;
    //member variables:
    //LEDs for cube detection
    //Motor for picking up cube
    
    
};