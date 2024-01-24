#include <Arduino.h>

#define LINESENSOR1 2
#define LINESENSOR2 3
#define LINESENSOR3 4
#define LINESENSOR4 5

void SetupLineSensor(int sensor_pin);
void SetupLineSensors();
int ReadLineSensorNum(int sensor_pin);
bool ReadLineSensor(int sensor_pin);