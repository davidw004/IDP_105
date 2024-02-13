#include <Arduino.h>
#include "line_follower.h"
#include "cube_retrieval.h"

//Line_Follower robot;
//Cube_Retrieval cube;
TOFSensor timeOF;
void setup() {
  Serial.begin(9600);
  //robot.setup();
  //cube.setup();
  timeOF.Setup();
  while (digitalRead(BUTTON1) != 1)
  {
    delay(50);
  }
  //robot.exitbox();
}
void loop()
{
  //robot.go();
  Serial.print(timeOF.GetDistance());
  Serial.print(" ");
  delay(250);
}
