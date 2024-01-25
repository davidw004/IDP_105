#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *myMotor = AFMS.getMotor(1);

void setup() {
  AFMS.begin();
  myMotor->setSpeed(150);
}

void loop() {
  myMotor->run(FORWARD);
}