#include "TOFSensor.h"

//No constructor
TOFSensor::TOFSensor()
{
}

//Setup TOF
void TOFSensor::Setup()
{
    //join i2c bus (address optional for master)
    Wire.begin();
    //Set I2C sub-device address
    sensor.begin(0x50);
    //Set to Back-to-back mode and high precision mode
    sensor.setMode(sensor.eContinuous, sensor.eHigh);
    //Laser rangefinder begins to work
    sensor.start();
}

//Return distance in cm
float TOFSensor::GetDistance()
{   
    return sensor.getDistance() / 10; // Sensor works in mm while Ultrasound works in cm
}
