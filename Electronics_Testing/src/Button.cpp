#include "Button.h"

Button::Button(int button_pin)
    :
    pin(button_pin)
{
}

void Button::Setup()
{
    pinMode(pin, INPUT);
}

bool Button::IsPressed()
{
    return digitalRead(pin) == HIGH;
}
