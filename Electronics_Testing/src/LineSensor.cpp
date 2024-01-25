#include "LineSensor.h"

int ReadLineSensorNum(int sensor_pin)
{
    return digitalRead(sensor_pin);
}

bool ReadLineSensor(int sensor_pin)
{
    return ( ReadLineSensorNum(sensor_pin) == 1 );
}

LineSensor::LineSensor(int sensor_pin)
    :
    pin(sensor_pin)
{
}

void LineSensor::Setup()
{
    pinMode(pin, INPUT); // declare LED as output
}
