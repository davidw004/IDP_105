#pragma once
#include "Arduino.h"
#include "route_planning.h"
#include "motor_setup.h"
#include "ArduinoPinout.h"
#include "cube_retrieval.h"

#define maxSpeedLeft 150
#define maxSpeedRight 150
#define baseSpeedLeft 150
#define baseSpeedRight 150
#define turnDelay 3000
#define continueDelay 1500

class Line_Follower {

    private:
        Cube_Retrieval cubeRetrieval;

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
        bool blockHard;
        uint8_t blocksCollected;
        uint8_t pos;
        uint32_t startTime;
        bool isPickingUpCube, isReturningCube;
        Adafruit_DCMotor *_leftMotor, *_rightMotor;
        const Decision * _currentRoute;
};