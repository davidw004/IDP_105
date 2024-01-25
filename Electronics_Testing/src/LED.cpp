#include "LED.h"

Led::Led(int Led_pin)
    :
    pin(Led_pin)
{
}

void Led::Setup()
{
    pinMode(pin, OUTPUT);
}

void Led::TurnOn()
{
    digitalWrite(pin, HIGH);
}

void Led::TurnOff()
{
    digitalWrite(pin, LOW);
}

void Led::Set(bool Value)
{
    if (Value) { TurnOn(); }
    else { TurnOff(); }
}
