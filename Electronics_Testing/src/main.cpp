#include "LineSensor.h"
#include <Arduino.h>

void setup()
{
    Serial.begin(9600); // Init the serial port
    SetupLineSensor(LINESENSOR1);
}

void loop()
{
    if (ReadLineSensor(LINESENSOR1))
    {
        Serial.print("White Detected\n");
    }
    else
    {
        Serial.print("No White Detected\n");
    }
    delay(500);
}
