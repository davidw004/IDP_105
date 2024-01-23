#include <Arduino.h>
#include "line_follower.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Line_Follower robot(6,5,75);
  //pinMode(13, OUTPUT);          // sets the digital pin 13 as output

}

void loop() {
  // put your main code here, to run repeatedly:

}