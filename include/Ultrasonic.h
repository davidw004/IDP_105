#include "Arduino.h"
#define MAX_RANG (520)//the max measurement value of the module is 520cm(a little bit longer than effective max range)
#define ADC_SOLUTION (1023.0)//ADC accuracy of Arduino UNO is 10bit

class Ultrasonic
{
    public:
    Ultrasonic(int ultrasonic_pin);
    void Setup();
    float Read();
    
    private:
    int pin;
};