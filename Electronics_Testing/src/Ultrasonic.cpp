#include "Ultrasonic.h"

Ultrasonic::Ultrasonic(int ultrasonic_pin)
    :
    pin(ultrasonic_pin)
{
}

void Ultrasonic::Setup()
{
    
}

float Ultrasonic::Read()
{
    return analogRead(pin) * MAX_RANG / ADC_SOLUTION;
}
