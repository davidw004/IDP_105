//#include "LineSensor.h"
#include "ArduinoPinout.h"
#include "ServoIDP.h"
#include "Button.h"
#include "TOFSensor.h"
#include "Ultrasonic.h"
#include <Arduino.h>

ServoIDP testServo = ServoIDP(SERVO1);
Button testButton = Button(BUTTON1);
TOFSensor TOF = TOFSensor();
Ultrasonic USDist = Ultrasonic(ULTRASONIC);

void setup()
{
    Serial.begin(9600); // Init the serial port
    testButton.Setup();
    TOF.Setup();
    delay(100);
}

float dist_t, sensity_t;

void loop()
{
    if (testButton.IsPressed())
    {
        Serial.print(USDist.Read(), 0);
        Serial.print("cm (Ultrasound)  ");
        Serial.print(TOF.GetDistance());
        Serial.print("(cm) TOF");
    }
    delay(500);
}
