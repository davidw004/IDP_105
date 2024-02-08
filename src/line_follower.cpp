#include "Line_Follower.h"

Line_Follower::Line_Follower()
{
    maxSpeedLeft = 200;
    maxSpeedRight = 200;
    baseSpeedLeft = 175;
    baseSpeedRight = 175;
    turnSpeed = 125;
    baseSweepSpeed = 50;
    blocksCollected = 0;
    pos = 0;
    continueDelay = 500;
    _correctionTime = 110;
    _turnTime = 750;
    _reverseTime = 2500;
    _exitBoxTime = 400;
    _currentRoute = Routes::routeOne;
}

void Line_Follower::setup()
{
    pinMode(LINESENSOR1, INPUT);
    pinMode(LINESENSOR2, INPUT);
    pinMode(LINESENSOR3, INPUT);
    pinMode(LINESENSOR4, INPUT);

    pinMode(REDLED, OUTPUT);
    pinMode(GREENLED, OUTPUT);
    pinMode(BLUELED, OUTPUT);
//this has been moved
    

    _leftMotor = AFMS.getMotor(1);
    _rightMotor = AFMS.getMotor(2);
    cubeRetrieval.setup();
    
    AFMS.begin();
    cubeRetrieval.raiseClaw();
    _leftMotor->setSpeed(baseSpeedLeft);
    _rightMotor->setSpeed(baseSpeedRight);

    timeFactor = 0.25; //modify this value according to line adjustment required. 
}

//TUNE TIME OF SWEEP AND MOTOR SPEEDS 
void Line_Follower::sweep(){ //FINDS LINE. ONLY WORKS IF LINE IS BETWEEN INNER AND OUTER SENSORS, AND AWAY FROM A JUNCTION 
    _leftMotor -> setSpeed(baseSweepSpeed);
    _rightMotor -> setSpeed(baseSweepSpeed);
    _leftMotor -> run(FORWARD);
    _rightMotor -> run(BACKWARD); //sweeping left    
    startTime = millis();
    while ((millis()-startTime) < 1000){  //sweep for a second 
        _extremeLeftReading = digitalRead(LINESENSOR1);
        _extremeRightReading = digitalRead(LINESENSOR4); 
        if (_extremeRightReading == 1 ){ //line to the right of the robot 
            while(digitalRead(LINESENSOR3)== 0){
                _leftMotor -> run(BACKWARD);
                _rightMotor -> setSpeed(0.3 * baseSweepSpeed);
            }
        }
        else if (_extremeLeftReading == 1){ //line to left of robot
            while(digitalRead(LINESENSOR2)== 0) { //drive torwards line until hit. 
                _leftMotor -> run(BACKWARD);
                _leftMotor -> setSpeed(baseSweepSpeed);
            }
        }
    }    
}

//TUNE DELAY 
void Line_Follower::exitbox()
{
    
    //Turn LED on
    digitalWrite(BLUELED, HIGH);

    //Run motors forwards

    while (_leftReading == 0 && _rightReading ==0){ //drive up to box edge
        _leftMotor-> run(BACKWARD); //motors are connected in reverse
        _rightMotor-> run(BACKWARD);
        _leftReading = digitalRead(LINESENSOR2);
        _rightReading = digitalRead(LINESENSOR3);
    }
    //Wait until exited box, ie central line sensors are past the white line
    delay(_exitBoxTime); //This puts the line sensors just over the white line. Fine tune duration
    _leftMotor -> run(RELEASE);//stop car driving away. 
    _rightMotor -> run(RELEASE);
    /*if (digitalRead(LINESENSOR2) == 0 && digitalRead(LINESENSOR3) == 0){ //if not on line, find it. 
        Serial.print("sweeping now ");
        sweep();
    }*/
}

//TUNE timeFactor VARIABLE. LINE SENSOR INITIALISATION MOVED TO setup().
void Line_Follower::go()
{
    _extremeLeftReading = digitalRead(LINESENSOR1);
    _leftReading = digitalRead(LINESENSOR2);
    _rightReading = digitalRead(LINESENSOR3);
    _extremeRightReading = digitalRead(LINESENSOR4); 
    //should this code be deleted
    if (_extremeLeftReading == 1 || _extremeRightReading == 1){
        junction();
    }
       
    //If both middle sensors black keep driving at maxspeed
    if (_leftReading == 1 && _rightReading == 1)
    {        
        _leftMotor -> setSpeed(baseSpeedLeft);
        _rightMotor -> setSpeed(baseSpeedRight);
        //Run motors forwards
        _leftMotor->run(BACKWARD); //motors are connected in reverse
        _rightMotor->run(BACKWARD); //motors are connected in reverse
    }
    else if (_leftReading == 1 && _rightReading == 0){ //If left high (white) and right low (black) then change motor speed to turn left
        turnStart = millis(); //get time at start and end of turn. 
        _leftMotor -> setSpeed( 0.5 * baseSpeedLeft);
        _rightMotor -> setSpeed(baseSpeedRight);
        //Run motors forwards
        _leftMotor->run(BACKWARD); //motors are connected in reverse
        _rightMotor->run(BACKWARD); //motors are connected in reverse
        while (_leftReading == 1 && _rightReading == 0)
        { //run line correction until sensor is back on white. 
            _leftReading = digitalRead(LINESENSOR2);
            _rightReading = digitalRead(LINESENSOR3);
            Serial.print("correction from right");
        }
        //turnEnd = millis();
        //while ((millis()-turnEnd)<((turnEnd-turnStart)*timeFactor)){ //reRun motors in opposite turn direction to correct. 
        _leftMotor -> setSpeed(baseSpeedLeft);//i was reluctant to loop these terms, but i didnt want to mess up timing variable 
        _rightMotor -> run(FORWARD); //this is kind of wack but i think it will work
        Serial.print("second correction ");
        delay(_correctionTime);
        _rightMotor -> run(RELEASE);
        _leftMotor -> run(RELEASE);
    }
    
    else if (_leftReading == 0 && _rightReading == 1)
    { //If left high and right low then change motor speed to turn left
        turnStart = millis();
        _leftMotor -> setSpeed(baseSpeedLeft);
        _rightMotor -> setSpeed(0.5 * baseSpeedRight);
        //Run motors forwards
        _leftMotor->run(BACKWARD); //motors are connected in reverse
        _rightMotor->run(BACKWARD); //motors are connected in reverse
        while (_leftReading == 0 && _rightReading == 1)
        {                
            _leftReading = digitalRead(LINESENSOR2);
            _rightReading = digitalRead(LINESENSOR3);
            Serial.print("correction from right");
        }
        //turnEnd = millis();

        _rightMotor -> setSpeed(baseSpeedRight);
        _leftMotor -> run(FORWARD) ;
        Serial.print("second correction ");
        delay(_correctionTime);
        _rightMotor -> run(RELEASE);
        _leftMotor -> run(RELEASE);
    }
    else
    {
        _leftMotor -> run(RELEASE);
        _rightMotor -> run(RELEASE);
        digitalWrite(BLUELED, LOW);
    }

    //For error, keep track of which value was last white and recorrecr
}

void Line_Follower::leftTurn()
{   
    _rightMotor -> setSpeed(turnSpeed);
    _leftMotor -> setSpeed(turnSpeed);
    _leftMotor->run(FORWARD);
    _rightMotor->run(BACKWARD);
    delay(800);
    _extremeLeftReading = digitalRead(LINESENSOR1);
    while(_extremeLeftReading != 1)
    {
        _leftMotor->run(FORWARD);
        _rightMotor->run(BACKWARD);   
        _extremeLeftReading = digitalRead(LINESENSOR1);   
    }
    //_rightMotor -> setSpeed(0.6 * baseSpeedRight);
    _rightMotor -> run(RELEASE);
    _leftMotor -> run(BACKWARD);
    delay(_turnTime);

}

void Line_Follower::rightTurn()
{
    _leftMotor -> setSpeed(turnSpeed);
    _rightMotor -> setSpeed(turnSpeed);
    _leftMotor->run(BACKWARD);
    _rightMotor->run(FORWARD);   
    delay(800);
    _extremeRightReading = digitalRead(LINESENSOR4);
    while(_extremeRightReading != 1)
    {
        _leftMotor->run(BACKWARD);
        _rightMotor->run(FORWARD); 
        _extremeRightReading = digitalRead(LINESENSOR4);
    }
    //_leftMotor -> setSpeed(0.6 * baseSpeedLeft);
    _leftMotor -> run(RELEASE);
    _rightMotor -> run(BACKWARD);
    delay(_turnTime);
}

void Line_Follower::straight()
{
    _leftMotor -> setSpeed(baseSpeedLeft);
    _rightMotor -> setSpeed(baseSpeedRight);
    _leftMotor->run(BACKWARD);  // Replace if needed
    _rightMotor->run(BACKWARD);
    delay(continueDelay);
}

void Line_Follower::turn180()
{   
    _leftMotor -> setSpeed(turnSpeed);
    _rightMotor -> setSpeed(turnSpeed);
    _leftMotor -> run(FORWARD);
    _rightMotor -> run(BACKWARD);
    turnStart = millis();
    while (_extremeLeftReading == 0)
    { //TIME TURN TO 90 DEGREES.
    _extremeLeftReading = digitalRead(LINESENSOR1);
    }
    turnMid = millis();
    delay(turnMid - turnStart); //CONTINUES TURN FOR SAME TIME IT TOOK TO REACH 90 DEGREES
    _leftMotor -> run(RELEASE);
    _rightMotor -> run(RELEASE);
}

void Line_Follower::approachCube(uint32_t duration) //change duration to distance using ultrasound
{
    //Currently just hard coding the distance from final turn to the block. Will likely need editing
    _leftMotor -> run(BACKWARD);
    _rightMotor -> run(BACKWARD);
    delay(100);
    Serial.print("in approachcube ");
    for(uint32_t tStart = millis();  (millis()-tStart) < duration;)
    {
        go();
    }
    _leftMotor -> run(RELEASE);
    _rightMotor -> run(RELEASE);

}

void Line_Follower::junction()
{
    switch (_currentRoute[pos])
    {  
        case LEFT:
        {
            leftTurn();
            break;
        }
        case RIGHT:
        {
            rightTurn();
            break;
        }
        case STRAIGHT:
        {
            straight();
            break;
        }
        default:
        {
            break;
        }
    }
    pos++;
    switch(_currentRoute[pos])
    {   
        case LEFT:
        {
            _rightMotor -> setSpeed(baseSpeedRight);
            _leftMotor -> setSpeed(baseSpeedLeft);
            _leftMotor->run(BACKWARD);  // Replace if needed
            _rightMotor->run(BACKWARD);
            delay(250);
            break;
        }

        case RIGHT:
        {
            _rightMotor -> setSpeed(baseSpeedRight);
            _leftMotor -> setSpeed(baseSpeedLeft);
            _leftMotor->run(BACKWARD);  // Replace if needed
            _rightMotor->run(BACKWARD);
            delay(250);
            break;
        }
        case BLOCK:
        {   //lower motors
            _leftMotor->run(RELEASE);
            _rightMotor->run(RELEASE);
            cubeRetrieval.prepare();
            if (blocksCollected == 0) approachCube(200);
            else if (blocksCollected == 1) approachCube(200);
            //Collect block (call function as friend function)
            blockHard =  cubeRetrieval.pickUp();
            if (blockHard) digitalWrite(REDLED, HIGH);
            else digitalWrite(GREENLED, HIGH);
            delay(5000);

            digitalWrite(REDLED, LOW);
            digitalWrite(GREENLED, LOW);

            blocksCollected++;
            //Select route home based on current array and blockHard
            if (blockHard)
            {   
                if (blocksCollected == 1){_currentRoute = Routes::returnOneRed;}
                else if (blocksCollected == 2) {_currentRoute = Routes::returnTwoRed;}
            }
            else
            {
                if (blocksCollected == 1){_currentRoute = Routes::returnOneGreen;}
                else if (blocksCollected == 2) {_currentRoute = Routes::returnTwoGreen;}
            }
            pos = 0;
            turn180();
            break;
        }
        case HOME:
        {
            approachCube(2000); //this distance will change to ultrasound reading
            _leftMotor->run(RELEASE);
            _rightMotor->run(RELEASE);
            //Drop off cube code (include travel certain duration)
            cubeRetrieval.dropOff();
            //If blocksCollected = 1,2 etc select route to next block

            switch (blocksCollected)
            {
                case 1:
                {
                    if (blockHard) _currentRoute = Routes::routeTwoRed;
                    else _currentRoute = Routes::routeTwoGreen;
                    break;
                }
                /*case 2:
                {
                    if (blockHard) _currentRoute = Routes::routeTwoRed;
                    else _currentRoute = Routes::routeTwoGreen;
                    break;
                }*/
                default:
                {
                    break;
                }
                
            }
            _leftMotor -> run(FORWARD);
            _rightMotor -> run(FORWARD);
            delay(_reverseTime);
            _rightMotor -> run(RELEASE);
            _leftMotor -> run(RELEASE);

            cubeRetrieval.raiseClaw();
            turn180();
            pos = 0;
            break;
        }

        default:
        {
            break;
        }
    }
}

void Line_Follower::stop()
{
    _rightMotor -> run(RELEASE);
    _leftMotor -> run(RELEASE);
} 