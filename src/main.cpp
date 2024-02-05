#include <Arduino.h>
#include "line_follower.h"
#include "cube_retrieval.h"
#include "Ultrasonic.h"

Line_Follower robot;
Ultrasonic USonicDist(ULTRASONIC);
void setup() {
  Serial.begin(9600);
  robot.setup();

  //cube.setup();
  //robot.exitbox();

}
void loop(){
    robot.go();
}
