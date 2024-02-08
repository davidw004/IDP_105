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
        void exitCorrection();
        void straight();
        void turn180();
        void approachCube(uint32_t duration);
        void approachHome(uint32_t distance);
        void stop();
        void ledsOff();
        void junction();

    private:
        //Sensor values
        uint8_t _extremeLeftReading, _leftReading, _rightReading, _extremeRightReading;
        uint32_t _ultrasoundReading;

        //Motor speeds
        uint8_t maxSpeedLeft, maxSpeedRight;
        uint8_t baseSpeedLeft, baseSpeedRight;
        uint8_t baseSweepSpeed;
        uint8_t turnSpeed;
        
        //Timings for functions
        uint16_t _correctionTime;
        uint16_t _turnTime, _reverseTime;
        uint16_t _exitBoxTime;

        //Time for within functions
        uint32_t _startTime;
        uint32_t _turnStart, _turnEnd, _turnMid;
        uint8_t _timeFactor;
        uint32_t _turnDelay, _continueDelay;
        
        //Block detection and decision making
        bool _blockHard;
        uint8_t _blocksCollected;
        uint8_t pos;
        Adafruit_DCMotor *_leftMotor, *_rightMotor;
        const Decision *_currentRoute;
};