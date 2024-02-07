#include <Arduino.h>
#include "line_follower.h"
#include "cube_retrieval.h"

Line_Follower robot;
//Cube_Retrieval cube;
void setup() {
  Serial.begin(9600);
  Serial.print("Setup");

  //Set pinmode for moving led
  pinMode(BLUELED, OUTPUT);
  pinMode(GREENLED, OUTPUT);
  pinMode(BUTTON1, INPUT);

  robot.setup();
  while (BUTTON1 == LOW){
    delay(1000);
  }
  robot.exitbox();

  //cube.setup();
}
void loop()
{

  robot.go();
  //cube.test();
}
