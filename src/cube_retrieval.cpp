#include "cube_retrieval.h"
#include "ArduinoPinout.h"

Cube_Retrieval::Cube_Retrieval()
{
}

void Cube_Retrieval::setup(){
    _clawMotor = AFMS.getMotor(3);
    _clawMotor -> setSpeed(baseSpeed);
    AFMS.begin();
    pinMode(BUTTON1, INPUT);
    printf("in cube setup");
}

void Cube_Retrieval::test(){
    bool reading = digitalRead(BUTTON1);

            if (reading) {
                 _clawMotor -> run(FORWARD);
                 printf("BUTTON HIGH");
            }
            else {
                _clawMotor -> run(BACKWARD);
                printf("BUTT LOW");
            }
   /* }
    if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();

    if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
        if (reading != buttonState) {
            buttonState = reading;

            // only toggle the LED if the new button state is HIGH
            if (buttonState == HIGH) {
                 _clawMotor -> run(FORWARD);
            }
            else {
                _clawMotor -> run(BACKWARD);
            }
    }
  }
  }*/
}

void Cube_Retrieval::collect(){
    _clawMotor -> setSpeed(closingSpeed);
    startTime = millis();
    while ((millis()- startTime) < 1000) {
        _clawMotor -> run(FORWARD);
    }

    _clawMotor -> run(RELEASE);

    }
void Cube_Retrieval::fetch1(){

    /* go forward specifed distance by running line 
    follower code for a set amount of time and calling void. 
    Run written block collection code (written in this class)
    before running line follower reverse code(needs to be 
    written)*/
    /* run function which determines cube colour. alternitvely
    run cube colour code here and store it in a global variable*/
}
