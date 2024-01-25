#include "Arduino.h"
#include "Wire.h"
#include "DFRobot_VL53L0X.h"

class TOFSensor
{
    public:
    TOFSensor();
    void Setup();
    int GetDistance();
    
    private:
    int pin;
    DFRobot_VL53L0X sensor;
};