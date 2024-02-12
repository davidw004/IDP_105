#include "Line_Follower.h"

Line_Follower::Line_Follower()
{
    maxSpeed = 200;
    baseSpeed = 175;
    baseSpeed = 175;
    turnSpeed = 125;
    baseSweepSpeed = 50;
    _correctionFactor = 1.0;
    _blocksCollected = 0;
    _continueDelay = 500;
    _turnDelay = 800;
    _correctionTime = 20;
    _turnTime = 500;
    _reverseTime = 2500;
    _exitBoxTime = 300;
    _timeFactor = 0.25; //modify this value according to line adjustment required.

    _currentRoute = Routes::CollectBlockOne;
    pos = 0;
}

void Line_Follower::setup()
{
    pinMode(LINESENSOR1, INPUT);
    pinMode(LINESENSOR2, INPUT);
    pinMode(LINESENSOR3, INPUT);
    pinMode(LINESENSOR4, INPUT);
    pinMode(BUTTON1, INPUT);

    pinMode(REDLED, OUTPUT);
    pinMode(GREENLED, OUTPUT);
    pinMode(BLUELED, OUTPUT);

    _leftMotor = AFMS.getMotor(1);
    _rightMotor = AFMS.getMotor(2);
    cubeRetrieval.setup();
}

void Line_Follower::readAllLFSensors()
{
    _extremeLeftReading = digitalRead(LINESENSOR1);
    _leftReading = digitalRead(LINESENSOR2);
    _rightReading = digitalRead(LINESENSOR3);
    _extremeRightReading = digitalRead(LINESENSOR4); 
}

//TUNE TIME OF SWEEP AND MOTOR SPEEDS 
void Line_Follower::sweep(){ //FINDS LINE. ONLY WORKS IF LINE IS BETWEEN INNER AND OUTER SENSORS, AND AWAY FROM A JUNCTION 
    motorDrive(-baseSweepSpeed, baseSweepSpeed); //sweeps left 
    _startTime = millis();
    for(uint32_t _startTime = millis();  (millis() - _startTime) < 1000;) //Replace this with while ultrasound less than distance passed to func
    {
        readAllLFSensors();
        if (_extremeRightReading == 1 ){ //line to the right of the robot 
            while(digitalRead(LINESENSOR3) == 0){
                _leftMotor -> run(BACKWARD);
                _rightMotor -> setSpeed(0.3 * baseSweepSpeed);
            }
        }
        else if (_extremeLeftReading == 1){ //line to left of robot
            while(digitalRead(LINESENSOR2) == 0) { //drive torwards line until hit. 
                _leftMotor -> run(BACKWARD);
                _leftMotor -> setSpeed(baseSweepSpeed);
            }
        }
    }
    /*while ((millis()-_startTime) < 1000){  //sweep for a second 

    } */   
}

//TUNE DELAY 
void Line_Follower::exitbox()
{
    cubeRetrieval.raiseClaw();
    //Turn LED on
    digitalWrite(BLUELED, HIGH);

    //Run motors forwards

    while (_extremeLeftReading == 0 && _extremeRightReading == 0){ //drive up to box edge
        motorDrive(baseSpeed, baseSpeed);
        readAllLFSensors();
        Serial.print("exiting box ");
    }
    Serial.print("exited box and waiting ");
    //Wait until exited box, ie central line sensors are past the white line
    delay(_exitBoxTime); //This puts the line sensors just over the white line. Fine tune duration
    /*if (digitalRead(LINESENSOR2) == 0 && digitalRead(LINESENSOR3) == 0){ //if not on line, find it. 
        Serial.print("sweeping now ");
        sweep();
    }*/
}

void Line_Follower::motorDrive(uint8_t lspeed, uint8_t rspeed)
{
    _leftMotor -> setSpeed(abs(lspeed));
    _rightMotor -> setSpeed(abs(rspeed));

    if (lspeed < 0){_leftMotor -> run(FORWARD);}
    else if (lspeed > 0){_leftMotor -> run(BACKWARD);}
    else if (lspeed = 0){_leftMotor -> run(RELEASE);}

    if (rspeed < 0){_rightMotor -> run(FORWARD);}
    else if (rspeed > 0){_rightMotor -> run(BACKWARD);}
    else if (rspeed = 0){_rightMotor -> run(RELEASE);}
}

//TUNE _timeFactor VARIABLE. LINE SENSOR INITIALISATION MOVED TO setup().
void Line_Follower::go()
{
    readAllLFSensors();
    
    /*if (_extremeLeftReading == 1 || _extremeRightReading == 1)
    {
        junction();
    }*/

    //If both middle sensors black keep driving at maxspeed
    if (_leftReading == 1 && _rightReading == 1)
    {        
        motorDrive(baseSpeed, baseSpeed);
    }

    else if (_leftReading == 1 && _rightReading == 0){ //If left high (white) and right low (black) then change motor speed to turn left
        Serial.print("adjusting left ");
        adjust(LEFT);
    }
    else if (_leftReading == 0 && _rightReading == 1)
    { //If left high and right low then change motor speed to turn left
        Serial.print("adjusting right ");
        adjust(RIGHT);
    }
    else if (_leftReading == 0 && _rightReading == 0)
    {
        motorDrive(baseSpeed, baseSpeed);
    }

}

void Line_Follower::adjust(int direction)
{
    //If left high (white) and right low (black) then change motor speed to turn left
    //_turnStart = millis(); //get time at start and end of turn. 
    if (direction = LEFT)
    {   
        Serial.print("in adjusting left block ");
        motorDrive(0, baseSpeed);       
    }

    else if (direction = RIGHT)
    {
        Serial.print("in adjusting right block ");
        motorDrive(baseSpeed, 0);  
    }
    //delay(_correctionTime);

    /*  _turnStart = millis(); //get time at start and end of turn. 
        while (_leftReading == 1 && _rightReading == 0)
        { //run line correction until sensor is back on white. 
            _leftReading = digitalRead(LINESENSOR2);
            _rightReading = digitalRead(LINESENSOR3);
        }
        //_turnEnd = millis();
        //while ((millis()-_turnEnd)<((_turnEnd-_turnStart)*_timeFactor)){ //reRun motors in opposite turn direction to correct. 
        //_leftMotor -> setSpeed(baseSpeed);
        //_rightMotor -> run(FORWARD);*/
}

void Line_Follower::leftTurn()
{   
    motorDrive(-turnSpeed, turnSpeed);
    delay(_turnDelay);
    _extremeLeftReading = digitalRead(LINESENSOR1);
    while(_extremeLeftReading != 1)
    {
        _extremeLeftReading = digitalRead(LINESENSOR1);   
    }
    //_rightMotor -> setSpeed(0.6 * baseSpeed);
    
    motorDrive(turnSpeed, 0);
    delay(_turnTime);
}

void Line_Follower::rightTurn()
{
    motorDrive(turnSpeed, -turnSpeed); 
    delay(_turnDelay);
    _extremeRightReading = digitalRead(LINESENSOR4);
    while(_extremeRightReading != 1)
    {
        _extremeRightReading = digitalRead(LINESENSOR4);
    }
    //_leftMotor -> setSpeed(0.6 * baseSpeed);
    motorDrive(0, turnSpeed);
    delay(_turnTime);
}

void Line_Follower::exitCorrection()
{
    motorDrive(baseSpeed, baseSpeed);
    delay(200);
}

void Line_Follower::straight()
{
    motorDrive(baseSpeed, baseSpeed);
    delay(_continueDelay);
}

void Line_Follower::turn180()
{      
    motorDrive(-turnSpeed, turnSpeed);
    _turnStart = millis();
    while (_extremeLeftReading == 0)
    { //TIME TURN TO 90 DEGREES.
    _extremeLeftReading = digitalRead(LINESENSOR1);
    }
    _turnMid = millis();
    delay(_turnMid - _turnStart); //CONTINUES TURN FOR SAME TIME IT TOOK TO REACH 90 DEGREES
    stop();
}

void Line_Follower::approachCube(uint32_t duration) //change duration to distance using ultrasound
{
    //Currently just hard coding the distance from final turn to the block. Will likely need editing
    motorDrive(baseSpeed, baseSpeed);
    delay(100); //Clearing the white line to prevent junction detection when go() called
    for(uint32_t tStart = millis();  (millis()-tStart) < duration;) //Replace this with while ultrasound less than distance passed to func
    {
        go();
    }
    stop();

}

void Line_Follower::approachHome(uint32_t distance)
{
    motorDrive(baseSpeed, baseSpeed);
    delay(100); //Clearing the white line to prevent junction detection when go() called
    for(uint32_t tStart = millis();  (millis()-tStart) < 1000;) //Replace this with while ultrasound less than distance passed to func
    {
        go();
    }
    while(_ultrasoundReading < 25 || _ultrasoundReading > 30) //Replace this with while ultrasound less than distance passed to func
    {
        go();
        _ultrasoundReading = analogRead(ULTRASOUND); //May not be correct
    }
    //Stop motors, reached intended coordinate
    stop();
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
        {
            stop();
            cubeRetrieval.prepare(7500); //7500 for commercial zone pickup, change to limit switch

            if (_blocksCollected == 0) approachCube(200);
            else if (_blocksCollected == 1) approachCube(100); //Instead change duration 100 to a distance (while )
            //Either more else ifs or potentially add new switch case for industrial blocks

            _blockHard =  cubeRetrieval.pickUp();
            ledsOff();

            _blocksCollected++;
            //Select route home based on current array and blockHard
            if (_blockHard)
            {   
                if (_blocksCollected == 1){_currentRoute = Routes::BringBlockOneToRed;}
                else if (_blocksCollected == 2) {_currentRoute = Routes::BringBlockTwoToRed;}
            }
            else
            {
                if (_blocksCollected == 1){_currentRoute = Routes::BringBlockOneToGreen;}
                else if (_blocksCollected == 2) {_currentRoute = Routes::BringBlockTwoToGreen;}
            }
            pos = 0;
            turn180();
            break;
        }
        
        case HOME:
        {
            approachHome(2000); //this distance will change to ultrasound reading to wall at dropoff position
            
            cubeRetrieval.dropOff();
            //If _blocksCollected = 1,2 etc select route to next block

            switch (_blocksCollected)
            {
                case 1:
                {
                    if (_blockHard) _currentRoute = Routes::CollectBlockTwoFromRed;
                    else _currentRoute = Routes::CollectBlockTwoFromGreen;
                    break;
                }
                /*case 2:
                {
                    if (_blockHard) _currentRoute = Routes::routeTwoRed;
                    else _currentRoute = Routes::routeTwoGreen;
                    break;
                }*/
                default:
                {
                    break;
                }
                
            }

            motorDrive(-baseSpeed, -baseSpeed);
            delay(_reverseTime);
            stop();

            cubeRetrieval.raiseClaw();
            turn180();
            pos = 0;
            break;
        }

        case ENTERSWAN:
        {
            //approach and collect from swan zone
            break;
        }

        case EXITSWAN:
        {
            // reverse until hit white line:
            while (_extremeLeftReading == 0 && _extremeRightReading == 0)
            {
                motorDrive(-baseSpeed, -baseSpeed);
            }
            motorDrive(0, 0);

            // turn to right on line for now:

            break;
        }

        case ENTERLUCOZADE:
        {
            //approach and collect from lucozade zone
            break;
        }
        
        case EXITLUCOZADE:
        {
            // reverse until hit white line:
            while (_extremeLeftReading == 0 && _extremeRightReading == 0)
            {
                motorDrive(-baseSpeed, -baseSpeed);
            }
            motorDrive(0, 0);
            // turn to left on line:


            break;
        }

        default:
        {
            exitCorrection();
            break;
        }
    }
}

void Line_Follower::stop()
{
    _rightMotor -> run(RELEASE);
    _leftMotor -> run(RELEASE);
}

void Line_Follower::ledsOff()
{
    digitalWrite(REDLED, LOW);
    digitalWrite(GREENLED, LOW);
}
