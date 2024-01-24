//#include "LineSensor.h"
#include "ArduinoPinout.h"
#include "ServoIDP.h"
#include <Arduino.h>

ServoIDP testServo = ServoIDP(SERVO1);

void setup()
{
    Serial.begin(9600); // Init the serial port
    testServo.Setup();
    delay(100);
}

void loop()
{
    testServo.turnSmoothly(180, 10);
    testServo.turnSmoothly(-180, 10);
    delay(100);
}
