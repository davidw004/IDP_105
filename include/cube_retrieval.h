#pragma once
#include "Arduino.h"
#include "ArduinoPinout.h"
#include <SPI.h>
#include "motor_setup.h"
#include "line_follower.h"
#include "Ultrasonic.h"

#define baseSpeed 250
#define closingSpeed 200
#define pickupDistance 10.0f
#define 3DPRINTEDBLOCK true
#define FOAMBLOCK false

class Cube_Retrieval 
{
    public:
    Cube_Retrieval();
    void setup(); 
    bool pickUp();
    void dropOff();
    void test();
    void approachCube(&Line_Follower LF, Ultrasonic* USonicSensor);

    private:
    //member variables:
    Adafruit_DCMotor *_clawMotor;
    uint8_t buttonState;
    bool atEnd;
    bool _blockType;
    uint32_t _pickupStart;
    uint32_t _pickupEnd;
    //LEDs for cube detection    
    
};