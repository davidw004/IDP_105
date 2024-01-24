#include "Arduino.h"
#include "Servo.h"

void SetupServo(int servo_pin);

class ServoIDP
{
    public:
    void ServoIDP(int servo_pin);
    void SetAngle(int degrees);
    void ChangeAngle(int deltaDegrees);
    void Update();
    void turnSmoothly(int deltaDegrees, int msPerDegree);

    int pos;
    private:
    Servo thisServo;

}