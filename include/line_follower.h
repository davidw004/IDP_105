#pragma once
#include "Arduino.h"
#include "route_planning.h"
#include "motor_setup.h"
#include "ArduinoPinout.h"
#include "cube_retrieval.h"
#include "TOFSensor.h"

// Defining directions
#define RIGHTTURN 1
#define LEFTTURN -1

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
        void motorDrive(int lspeed, int rspeed);
        void go();
        void timedGo(int duration, int speed);
        void adjust(int direction);
        void leftTurn();
        void rightTurn();
        void straight(uint16_t duration, int speed);
        void reverse(int duration, int speed);
        void turn180(int direciton);
        void turnFactory(int direction, bool OnJunction);
        void approachCube(uint32_t duration);
        void approachHome(float duration);
        void enterIndustrial(int duration, int durationPush);
        void reverseToLine();
        void stop();
        void ledsOff();
        void junction();
        void swanTest();

    private:
        //Sensor values
        uint8_t _extremeLeftReading, _leftReading, _rightReading, _extremeRightReading;
        float _ultrasoundReading;

        //Motor speeds
        uint8_t maxSpeed;
        uint8_t baseSpeed, approachSpeed;
        uint8_t baseSweepSpeed;
        uint8_t turnSpeed;
        uint8_t approachIndustrialSpeed;
        float _correctionFactor;
        
        //Timings for functions
        uint16_t _correctionTime;
        uint16_t _reverseTime;
        uint16_t _exitBoxTime;
        uint16_t _commercialPrepare;

        //Time for within functions
        uint32_t _startTime;
        uint32_t _turnStart, _turnEnd, _turnMid;
        uint8_t _timeFactor;
        uint32_t _turnDelay, _continueDelay;
        uint32_t appoachHomeDuration;
        uint32_t approachSwanDuration;
        uint32_t approachLucozadeDuration;
        
        //Block detection and decision making
        bool _blockHard;
        uint8_t _blocksCollected;
        uint8_t pos;
        Adafruit_DCMotor *_leftMotor, *_rightMotor;
        const Decision *_currentRoute;
        float swanDist, lucozadeDist;
};