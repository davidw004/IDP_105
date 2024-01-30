#pragma once
#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include <SPI.h>
#include "route_planning.h"
class Line_Follower {
    public:
        Line_Follower();
        void setup();
        void exitbox();
        void go();
        void stop();
        void junction();

    private:
        uint8_t _extremeLeftReading, _leftReading, _rightReading, _extremeRightReading;
        //uint8_t _left_weight;
        //uint8_t _right_weight;
        uint8_t maxSpeedLeft, maxSpeedRight;
        uint8_t baseSpeedLeft, baseSpeedRight;
        uint8_t blocksCollected;
        uint8_t pos;
        Adafruit_DCMotor *_leftMotor, *_rightMotor;
        const Decision * _currentRoute;
};