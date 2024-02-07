#include <Arduino.h>
#include "line_follower.h"
#include "cube_retrieval.h"

Line_Follower robot;
//Cube_Retrieval cube;
void setup() {
  Serial.begin(9600);
  Serial.print("Setup");
  //robot.setup();
  //robot.exitbox();
  //cube.setup();
}
void loop(){
  robot.go();
  //cube.test();
}
