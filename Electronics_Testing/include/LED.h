#include "Arduino.h"
class Led
{
    public:
    Led(int Led_pin);
    void Setup();
    void TurnOn();
    void TurnOff();
    void Set(bool Value);
    
    private:
    int pin;
};