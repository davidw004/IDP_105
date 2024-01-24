#include <Arduino.h>
#include "ArduinoPinout.h"

void SetupLineSensor(int sensor_pin);
void SetupLineSensors();
int ReadLineSensorNum(int sensor_pin);
bool ReadLineSensor(int sensor_pin);