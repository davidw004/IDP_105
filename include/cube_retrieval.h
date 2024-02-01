#pragma once
#include "Arduino.h"
#include "ArduinoPinout.h"
#include <SPI.h>
#include "motor_setup.h"

class Cube_Retrieval 
{
    public:
    Cube_Retrieval();
    void setup(); 
    void collect();
    void fetch1();
    void test();
    //Function for moving forward certain distance up to cube
    //Function for detecting type of cube and turning on light
    //Function for picking up cube

    private:
    //member variables:
    uint8_t baseSpeed;
    uint8_t closingSpeed;
    Adafruit_DCMotor *_clawMotor;
    uint8_t buttonState;
    //LEDs for cube detection
    //Motor for picking up cube
    
    
};