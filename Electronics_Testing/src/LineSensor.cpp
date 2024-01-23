#include "LineSensor.h"

void SetupSensor(int sensor_pin)
{
    pinMode(sensor_pin, INPUT); // declare LED as output
}

int ReadSensor(int sensor_pin)
{
    return digitalRead(sensor_pin);
}
