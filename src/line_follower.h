#pragma once
#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include <SPI.h>
#include "motor_setup.h"
#include "ArduinoPinout.h"
//#include "route_planning.h"
class Line_Follower {
    public:
        Line_Follower();
        void setup();
        void go();
        void stop();
        void junction();

    private:
        uint8_t _extremeLeftReading, _leftReading, _rightReading, _extremeRightReading;
        uint8_t _left_weight;
        uint8_t _right_weight;
        uint8_t maxSpeedLeft = 200;
        uint8_t maxSpeedRight = 200;
        uint8_t baseSpeedLeft = 150;
        uint8_t baseSpeedRight = 150;
        uint8_t blocksCollected = 0;
        Adafruit_DCMotor *_leftMotor;
        Adafruit_DCMotor *_rightMotor;
        //Decision *currentRoute = Routes::routeOne;
        //uint8_t pos = 0;
};