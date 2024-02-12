#pragma once
#include "Arduino.h"
#include "ArduinoPinout.h"
#include <SPI.h>
#include "motor_setup.h"
#define HARDBLOCK true
#define SOFTBLOCK false

class Cube_Retrieval 
{
    public:
    Cube_Retrieval();
    void setup();
    bool pickUp();
    bool detectCube();
    void dropOff();
    void test();
    void ultrasoundTest();
    float readUltrasound();
    void raiseClaw();
    void prepare();

    private:
    //member variables:
    uint8_t baseSpeed;
    uint8_t closingSpeed;
    uint32_t _pickupStart;
    uint32_t _pickupEnd;
    uint32_t _releaseTime;
    Adafruit_DCMotor *_clawMotor;
    bool _atEnd;
    bool _blockType;
};