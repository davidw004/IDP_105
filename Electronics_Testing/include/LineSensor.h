#include <Arduino.h>
#include "ArduinoPinout.h"

class LineSensor
{
    public:
    LineSensor(int sensor_pin);
    void Setup();
    bool White();

    private:
    int pin;
};