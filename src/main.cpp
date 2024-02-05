#include <Arduino.h>
#include "line_follower.h"
#include "cube_retrieval.h"
#include "Ultrasonic.h"

Ultrasonic USonicDist(ULTRASONIC);
Line_Follower robot(&USonicDist);
void setup() {
  USonicDist.Setup();
  Serial.begin(9600);
  robot.setup();

  //cube.setup();
  //robot.exitbox();

}
void loop(){
    robot.go();
}
