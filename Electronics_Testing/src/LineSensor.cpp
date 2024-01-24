#include "LineSensor.h"

void SetupLineSensor(int sensor_pin)
{
    pinMode(sensor_pin, INPUT); // declare LED as output
}

void SetupLineSensors()
{
    SetupLineSensor(LINESENSOR1);
    SetupLineSensor(LINESENSOR2);
    SetupLineSensor(LINESENSOR3);
    SetupLineSensor(LINESENSOR4);
}

int ReadLineSensorNum(int sensor_pin)
{
    return digitalRead(sensor_pin);
}

bool ReadLineSensor(int sensor_pin)
{
    return ( ReadLineSensorNum(sensor_pin) == 1 );
}