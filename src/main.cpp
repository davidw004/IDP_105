#include <Arduino.h>
#include "line_follower.h"
#include "cube_retrieval.h"
#include <Ticker.h>

Line_Follower robot;
Cube_Retrieval cube;
TOFSensor timeOF;

extern bool blue_led_on;
extern bool wheel_running;

void TickerFunc() 
{ // Timer1 interrupt service routine
  if (wheel_running) 
  {
    if (blue_led_on) 
    {
        digitalWrite(BLUELED, LOW);
    }
    else 
    {
        digitalWrite(BLUELED, HIGH);
    }
    blue_led_on = !blue_led_on;
  } 
  else 
  {
    digitalWrite(BLUELED, LOW);
  }
}
Ticker tickerObject(TickerFunc, 250);

void setup() {
  Serial.begin(9600);
  robot.setup();
  //cube.setup();
  //timeOF.Setup();
  while (digitalRead(BUTTON1) != 1)
  {
    delay(50);
  }
  tickerObject.start();
  robot.exitbox();
}
void loop()
{
  robot.go();
}
