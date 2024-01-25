#include "Arduino.h"
class Button
{
    public:
    Button(int button_pin);
    void Setup();
    bool IsPressed();
    
    private:
    int pin;
};