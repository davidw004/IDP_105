#include "ServoIDP.h"

void SetupServo(int servo_pin)
{
    Servo servo_to_setup;
    servo_to_setup.attach(servo_pin);
    return Servo;
}

ServoIDP::ServoIDP(int servo_pin)
{
    thisServo.attach(servo_pin);
    pos = 0;
}

ServoIDP::SetAngle(int degrees)
{
    pos = degrees;
}

ServoIDP::ChangeAngle(int deltaDegrees)
{
    pos += deltaDegrees;
}

ServoIDP::Update()
{
    thisServo.write(pos);
}

void ServoIDP::turnSmoothly(int deltaDegrees, int msPerDegree)
{
    int delta = 1;
    if (deltaDegrees < 0) { delta = -1; } 
    for (degree = 0; degree <= abs(deltaDegrees); degree += 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        ChangeAngle(delta);
        Update();
        delay(msPerDegree); // waits 15 ms for the servo to reach the position
    }
}
