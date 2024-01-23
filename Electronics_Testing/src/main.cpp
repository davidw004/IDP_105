#include "LineSensor.h"
#include <Arduino.h>

void setup()
{
    Serial.begin(9600); // Init the serial port
    SetupSensor(SENSOR1PIN);
}

void loop()
{
    Serial.print(ReadSensor(SENSOR1PIN));
    delay(100);
}
