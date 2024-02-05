#pragma once
#include "Arduino.h"
#include "route_planning.h"
#include "motor_setup.h"
#include "ArduinoPinout.h"
#include "cube_retrieval.h"

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
        uint8_t maxSpeedLeft, maxSpeedRight;
        uint8_t baseSpeedLeft, baseSpeedRight;
        bool blockHard;
        uint8_t blocksCollected;
        uint8_t pos;
        uint32_t startTime;
        uint8_t turnDelay, continueDelay;
        bool isPickingUpCube, isReturningCube;
        Adafruit_DCMotor *_leftMotor, *_rightMotor;
        const Decision *_currentRoute;
};