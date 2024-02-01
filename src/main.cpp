#include <Arduino.h>
#include "line_follower.h"
#include <SPI.h>
#include "ArduinoPinout.h"
#include "cube_retrieval.h"
#include "motor_setup.h"

Cube_Retrieval cube;
//Line_Follower robot;
void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);
  //robot.setup();
  cube.setup();
  return;
}

void loop() {
  cube.test();
}