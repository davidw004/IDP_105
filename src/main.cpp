#include <Arduino.h>
#include "line_follower.h"
#include <SPI.h>

Line_Follower robot;
//Line_Follower robot;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("Setup called");
  robot.setup();
  robot.exitbox();

}

void loop() {
  // put your main code here, to run repeatedly:
  //Line_Follower robot(6,5,75);
  robot.go();
}