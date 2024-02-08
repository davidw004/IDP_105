#include "Line_Follower.h"

Line_Follower::Line_Follower()
{
    maxSpeedLeft = 150;
    maxSpeedRight = 150;
    baseSpeedLeft = 125;
    baseSpeedRight = 125;
    baseSweepSpeed = 50;
    blocksCollected = 0;
    pos = 0;
    turnDelay = 1200;
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

    timeFactor = 0.3; //modify this value according to line adjustment required. 
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
    delay(500); //This puts the line sensors just over the white line. Fine tune duration
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
    if ((_extremeLeftReading == 1 || _extremeRightReading == 1)){

        if (isReturningCube == false){
        junction();
        }
        else if ((isReturningCube == true)){
        isReturningCube = false; //This will end the "go()" loop within the HOME switch case 
        return; //Returns from the go() function
        }
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
        _leftMotor -> setSpeed( 0.1 * baseSpeedLeft);
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
        turnEnd = millis();
        //while ((millis()-turnEnd)<((turnEnd-turnStart)*timeFactor)){ //reRun motors in opposite turn direction to correct. 
        _leftMotor -> setSpeed(baseSpeedLeft);//i was reluctant to loop these terms, but i didnt want to mess up timing variable 
        _rightMotor -> setSpeed(baseSpeedRight * 0.3); //this is kind of wack but i think it will work
        Serial.print("second correction ");
        delay((turnEnd-turnStart)*timeFactor);
    }
    
    else if (_leftReading == 0 && _rightReading == 1)
    { //If left high and right low then change motor speed to turn left
        turnStart = millis();
        _leftMotor -> setSpeed(baseSpeedLeft);
        _rightMotor -> setSpeed(0.1 * baseSpeedRight);
        //Run motors forwards
        _leftMotor->run(BACKWARD); //motors are connected in reverse
        _rightMotor->run(BACKWARD); //motors are connected in reverse
        while (_leftReading == 0 && _rightReading == 1)
        {                
            _leftReading = digitalRead(LINESENSOR2);
            _rightReading = digitalRead(LINESENSOR3);
            Serial.print("correction from right");
        }
        turnEnd = millis();
        _leftMotor -> setSpeed(0.3 * baseSpeedLeft) ;
        _rightMotor -> setSpeed(baseSpeedRight);
        Serial.print("second correction ");
        delay((turnEnd-turnStart)*timeFactor);
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
    _rightMotor -> setSpeed(baseSpeedRight);
    _leftMotor -> setSpeed(baseSpeedLeft);

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
    _rightMotor -> setSpeed(0.3 * baseSpeedRight);
    _rightMotor -> run(BACKWARD);
    _leftMotor -> run(BACKWARD);
    delay(400);
    _rightMotor -> setSpeed(baseSpeedRight);
    _leftMotor -> setSpeed(baseSpeedLeft);
    delay(400);
}

void Line_Follower::rightTurn()
{
    _leftMotor -> setSpeed(baseSpeedLeft);
    _rightMotor -> setSpeed(baseSpeedRight);
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
    _leftMotor -> setSpeed(0.3 * baseSpeedLeft);
    _leftMotor -> run(BACKWARD);
    _rightMotor -> run(BACKWARD);
    delay(400);
    _rightMotor -> setSpeed(baseSpeedRight);
    _leftMotor -> setSpeed(baseSpeedLeft);
    delay(400);
}

void Line_Follower::straight()
{
    _leftMotor -> setSpeed(baseSpeedLeft);
    _rightMotor -> setSpeed(baseSpeedRight);
    _leftMotor->run(FORWARD);  // Replace if needed
    _rightMotor->run(FORWARD);
    delay(continueDelay);
}

void Line_Follower::turn180()
{
    turnStart = millis();
    _leftMotor -> run(FORWARD);
    _rightMotor -> run(BACKWARD);
    while (_extremeLeftReading == 0){ //TIME TURN TO 90 DEGREES.
    _extremeLeftReading = digitalRead(LINESENSOR1);
    }
    turnMid = millis();
    delay(turnMid - turnStart); //CONTINUES TURN FOR SAME TIME IT TOOK TO REACH 90 DEGREES
    _leftMotor -> run(RELEASE);
    _rightMotor -> run(RELEASE);
}

void Line_Follower::approachCube()
{
    //Currently just hard coding the distance from final turn to the block. Will likely need editing
    startTime == millis();
    while ((millis() - startTime) < 2000)
    {
        go(); //Will need fine tuning
    }
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
        case BLOCK:
        {   //lower motors
            cubeRetrieval.prepare();
            approachCube();
            _leftMotor->run(RELEASE);
            _rightMotor->run(RELEASE);
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
            isReturningCube = true;
            while (isReturningCube == true) //test if code will detect home section
            {
                go();
            }
            _leftMotor->run(RELEASE);
            _rightMotor->run(RELEASE);
            //Drop off cube code (include travel certain duration)
            cubeRetrieval.dropOff();

            //If blocksCollected = 1,2 etc select route to next block
            if (_currentRoute = Routes::returnOneRed){_currentRoute = Routes::routeTwoRed;}
            else if (_currentRoute = Routes::returnOneGreen) {_currentRoute = Routes::routeTwoGreen;}
            
            //Spin 180 to begin next path
            _leftMotor -> setSpeed(baseSpeedLeft);
            _rightMotor -> setSpeed(baseSpeedRight);
            _leftMotor->run(BACKWARD);  // Replace if needed
            _rightMotor->run(FORWARD);
            //wait duration of time
            delay(4000);
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