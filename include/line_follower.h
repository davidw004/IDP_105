#pragma once
#include "Arduino.h"
#include "route_planning.h"
#include "motor_setup.h"
#include "ArduinoPinout.h"
#include "cube_retrieval.h"
#include "TOFSensor.h"

// Defining directions
#define RIGHT 1
#define LEFT -1

class Line_Follower {

    private:
        Cube_Retrieval cubeRetrieval;
        TOFSensor TimeOfFlight;

    public:
        Line_Follower();
        void setup();
        void readAllLFSensors();
        void sweep();
        void exitbox();
        void motorDrive(uint8_t lspeed, uint8_t rspeed);
        void go();
        void adjust(int direction);
        void leftTurn();
        void rightTurn();
        void exitCorrection();
        void straight();
        void turn180();
        void turn90(bool left, bool OnJunction);
        void driveForwardBaseSpeed(int time_ms);
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
        uint8_t maxSpeed;
        uint8_t baseSpeed;
        uint8_t baseSweepSpeed;
        uint8_t turnSpeed;
        float _correctionFactor;
        
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