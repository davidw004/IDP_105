#pragma once
#include "Arduino.h"
#include "route_planning.h"
#include "motor_setup.h"
#include "ArduinoPinout.h"
#include "cube_retrieval.h"
#include "TOFSensor.h"

class Line_Follower {

    private:
        Cube_Retrieval cubeRetrieval;
        TOFSensor TimeOfFlight;

    public:
        Line_Follower();
        void setup();
        void sweep();
        void exitbox();
        void go();
        void leftTurn();
        void rightTurn();
        void straight();
        void turn180();
        void approachCube(uint32_t duration);
        void stop();
        void junction();

    private:
        uint8_t _extremeLeftReading, _leftReading, _rightReading, _extremeRightReading;
        uint8_t maxSpeedLeft, maxSpeedRight;
        uint8_t baseSpeedLeft, baseSpeedRight, baseSweepSpeed;
        uint8_t turnSpeed;
        
        uint32_t _correctionTime;
        uint32_t _turnTime, _reverseTime;
        uint32_t _exitBoxTime;

        bool blockHard;
        uint8_t blocksCollected;
        uint8_t pos;
        uint32_t startTime;
        uint32_t turnStart, turnEnd, turnMid;
        uint8_t timeFactor;
        uint32_t turnDelay, continueDelay;
        Adafruit_DCMotor *_leftMotor, *_rightMotor;
        const Decision *_currentRoute;
};