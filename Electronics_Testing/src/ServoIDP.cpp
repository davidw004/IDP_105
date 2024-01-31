#include "ServoIDP.h"

ServoIDP::ServoIDP(int servo_pin)
{
    pin = servo_pin;
    pos = 0;
}

void ServoIDP::Setup()
{
    thisServo.attach(pin);
    Update();
}

void ServoIDP::SetAngle(int degrees)
{
    pos = degrees;
}

void ServoIDP::ChangeAngle(int deltaDegrees)
{
    pos += deltaDegrees;
}

void ServoIDP::Update()
{
    if (pos > 180) { pos = 180; }
    if (pos < 0) { pos = 0; }
    thisServo.write(pos);
}

void ServoIDP::turnSmoothly(int deltaDegrees, int msPerDegree)
{
    int delta = 1;
    if (deltaDegrees < 0) { delta = -1; } 
    for (int degree = 0; degree <= abs(deltaDegrees); degree += 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        ChangeAngle(delta);
        Update();
        delay(msPerDegree); // waits 15 ms for the servo to reach the position
    }
}
