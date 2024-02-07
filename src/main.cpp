#include <Arduino.h>
#include "line_follower.h"
#include "cube_retrieval.h"

Line_Follower robot;
void setup() {
  Serial.begin(9600);
  robot.setup();
  robot.exitbox();
  
  //cube.testTimer();
}
void loop()
{
  robot.go();
}
