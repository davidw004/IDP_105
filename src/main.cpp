#include <Arduino.h>
#include "line_follower.h"
#include "cube_retrieval.h"

Line_Follower robot;
//Cube_Retrieval cube;
void setup() {
  Serial.begin(9600);
  robot.setup();
  //cube.setup();
  while (digitalRead(BUTTON1) != 1)
  {
    delay(50);
  }
  robot.exitbox();
}
void loop()
{
  robot.go();
}
