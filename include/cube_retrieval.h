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
    void dropOff();
    void test();

    private:
    //member variables:
    uint8_t baseSpeed;
    uint8_t closingSpeed;
    Adafruit_DCMotor *_clawMotor;
    uint8_t buttonState;
    bool atEnd;
    bool _blockType;
    uint32_t _pickupStart;
    uint32_t _pickupEnd;
    //LEDs for cube detection
};