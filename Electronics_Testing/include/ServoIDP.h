#include "Arduino.h"
#include "Servo.h"

class ServoIDP
{
    public:
    ServoIDP(int servo_pin);
    void Setup();
    void SetAngle(int degrees);
    void ChangeAngle(int deltaDegrees);
    void Update();
    void turnSmoothly(int deltaDegrees, int msPerDegree);

    int pos;
    private:
    int pin;
    Servo thisServo;

};