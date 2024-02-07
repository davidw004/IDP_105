#include "Line_Follower.h"

Line_Follower::Line_Follower()
{
    maxSpeedLeft = 150;
    maxSpeedRight = 150;
    baseSpeedLeft = 150;
    baseSpeedRight = 150;
    blocksCollected = 0;
    pos = 0;
    turnDelay = 3000;
    continueDelay = 1500;
    isPickingUpCube = false;
    isReturningCube = false;
    _currentRoute = Routes::routeOne;
}

void Line_Follower::setup()
{
    pinMode(LINESENSOR1, INPUT);
    pinMode(LINESENSOR2, INPUT);
    pinMode(LINESENSOR3, INPUT);
    pinMode(LINESENSOR4, INPUT);

    //Set pinmode for moving led
    pinMode(BLUELED, OUTPUT);

    _leftMotor = AFMS.getMotor(1);
    _rightMotor = AFMS.getMotor(2);
    cubeRetrieval.setup();
    TimeOfFlight.Setup()
    
    AFMS.begin();
    _leftMotor->setSpeed(baseSpeedLeft);
    _rightMotor->setSpeed(baseSpeedRight);
}

void Line_Follower::exitbox(){
    //Position box physically in the middle of start
    
    //Turn LED on
    digitalWrite(BLUELED, HIGH);

    //Run motors forwards
    _leftMotor->run(FORWARD); //motors are connected in reverse
    _rightMotor->run(FORWARD); //motors are connected in reverse

    //Wait until exited box, ie central line sensors are past the white line
    delay(2000); //Fine tune duration
    _leftMotor->run(RELEASE);
    _rightMotor->run(RELEASE); 

}

void Line_Follower::go() {
    Serial.print("Running");

    _extremeLeftReading = digitalRead(LINESENSOR1);
    _leftReading = digitalRead(LINESENSOR2);
    _rightReading = digitalRead(LINESENSOR3);
    _extremeRightReading = digitalRead(LINESENSOR4);
    
    //Run motors forwards
    _leftMotor->run(BACKWARD); //motors are connected in reverse
    _rightMotor->run(BACKWARD); //motors are connected in reverse

    if ((_extremeLeftReading == 1 || _extremeRightReading == 1)){

        if (isReturningCube == false){
        junction();
        }
        else if ((isReturningCube == true)){
        //Drop off cube code;
        isReturningCube = false;
        return;
        }
    }
   
    //If both middle sensors black keep driving at maxspeed
    if (_leftReading == 1 && _rightReading == 1){
        _leftMotor -> setSpeed(baseSpeedLeft);
        _rightMotor -> setSpeed(baseSpeedRight);
    }
    else if (_leftReading == 1 && _rightReading == 0){ //If left high and right low then change motor speed to turn left
        _leftMotor -> setSpeed(0);
        _rightMotor -> setSpeed(baseSpeedRight);
    }
    
    else if (_leftReading == 0 && _rightReading == 1){ //If left high and right low then change motor speed to turn left
        _leftMotor -> setSpeed(baseSpeedLeft);
        _rightMotor -> setSpeed(0);
    }
    else {
        _leftMotor -> setSpeed(0);
        _rightMotor -> setSpeed(0);
    }

    //For error, keep track of which value was last white and recorrecr
}


void Line_Follower::junction(){
    switch (_currentRoute[pos]){  
        case LEFT:
        {
            //Turn left code
            _leftMotor -> setSpeed(baseSpeedLeft);
            _rightMotor -> setSpeed(0.5 * baseSpeedRight);
            _leftMotor->run(FORWARD);  // Replace if needed
            _rightMotor->run(BACKWARD);  // Replace if needed
            //wait duration of time
            delay(turnDelay);
            pos++;
            break;
        }
        case RIGHT:
        {
            //Turn right code
            _leftMotor -> setSpeed(0.5 * baseSpeedLeft);
            _rightMotor -> setSpeed(baseSpeedRight);
            _leftMotor->run(BACKWARD);  // Replace if needed
            _rightMotor->run(FORWARD);
            //wait duration of time
            delay(turnDelay);
            pos++;
            break;
        }
        case STRAIGHT:
        {
            //Continue straight (prevent double detection)
            _leftMotor -> setSpeed(baseSpeedLeft);
            _rightMotor -> setSpeed(baseSpeedRight);
            _leftMotor->run(FORWARD);  // Replace if needed
            _rightMotor->run(FORWARD);
            delay(continueDelay);
            pos++;
            break;
        }
        default:{
            break;
        }
    }
        switch(_currentRoute[pos]){
        case BLOCK:
        {
            startTime == millis();
            while ((millis() - startTime) < 3000){
                go(); //Will need fine tuning
            }
            
            //Collect block (call function as friend function)
            blockHard =  cubeRetrieval.pickUp();
            blocksCollected++;

            //Select route home based on current array and blockHard
            if (blockHard){
                if (_currentRoute == Routes::routeOne){_currentRoute = Routes::returnOneGreen;}
                else if (_currentRoute == Routes::routeTwoGreen || _currentRoute == Routes::routeTwoRed) {_currentRoute = Routes::returnTwoGreen;}
            }
            else{
                if (_currentRoute == Routes::routeOne){_currentRoute = Routes::returnOneRed;}
                else if (_currentRoute == Routes::routeTwoGreen || _currentRoute == Routes::routeTwoRed) {_currentRoute = Routes::returnTwoRed;}
            }

            break;
        }
        case HOME:
        {
            isReturningCube = true;
            // Continue driving until 3 seconds elapsed
            while (isReturningCube == true){
                go();
            }
            
            //Drop off cube code (include travel certain duration)
            cubeRetrieval.dropOff();

            //Spin 180 to begin next path
            _leftMotor -> setSpeed(baseSpeedLeft);
            _rightMotor -> setSpeed(baseSpeedRight);
            _leftMotor->run(BACKWARD);  // Replace if needed
            _rightMotor->run(FORWARD);
            //wait duration of time
            delay(4000);
            //If blocksCollected = 1,2 etc select route to next block
            break;
        }
        default:{
            break;
        }
    }
}

void Line_Follower::stop() {
    _rightMotor -> run(RELEASE);
    _leftMotor -> run(RELEASE);
}