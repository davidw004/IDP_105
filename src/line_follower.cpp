#include "Line_Follower.h"

Line_Follower::Line_Follower()
{
    maxSpeed = 200;
    baseSpeed = 200;
    turnSpeed = 125;
    approachSpeed = 125;
    baseSweepSpeed = 75;
    _correctionFactor = 0.2;
    _blocksCollected = 0;
    _continueDelay = 200;
    _turnDelay = 750;
    _correctionTime = 20;
    _reverseTime = 300;
    _exitBoxTime = 200;
    _timeFactor = 0.25; //modify this value according to line adjustment required.
    _commercialPrepare = 7500;
    swanDist = 20;

    _currentRoute = Routes::CollectBlockOne; //changed to test
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
void Line_Follower::sweep()
{ //FINDS LINE. ONLY WORKS IF LINE IS BETWEEN INNER AND OUTER SENSORS, AND AWAY FROM A JUNCTION 
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
    delay(_exitBoxTime);
}

void Line_Follower::motorDrive(int lspeed, int rspeed)
{
    _leftMotor -> setSpeed(abs(lspeed));
    _rightMotor -> setSpeed(abs(rspeed));

    if (lspeed < 0){_leftMotor -> run(FORWARD);}
    else if (lspeed > 0){_leftMotor -> run(BACKWARD);}
    else if (lspeed == 0){_leftMotor -> run(RELEASE);}

    if (rspeed < 0){_rightMotor -> run(FORWARD);}
    else if (rspeed > 0){_rightMotor -> run(BACKWARD);}
    else if (rspeed == 0){_rightMotor -> run(RELEASE);}
}

//TUNE _timeFactor VARIABLE. LINE SENSOR INITIALISATION MOVED TO setup().
void Line_Follower::go()
{
    readAllLFSensors();
    
    if (_extremeLeftReading == 1 || _extremeRightReading == 1)
    {   
        Serial.print("junction detected");
        junction();
    }

    //If both middle sensors black keep driving at maxspeed
    if (_leftReading == 1 && _rightReading == 1)
    {        
        motorDrive(baseSpeed, baseSpeed);
    }

    else if (_leftReading == 1 && _rightReading == 0){ //If left high (white) and right low (black) then change motor speed to turn left
        Serial.print("adjusting left ");
        adjust(LEFTTURN);
    }
    else if (_leftReading == 0 && _rightReading == 1)
    { //If left high and right low then change motor speed to turn left
        Serial.print("adjusting right ");
        adjust(RIGHTTURN);
    }
    else if (_leftReading == 0 && _rightReading == 0)
    {
        motorDrive(baseSpeed, baseSpeed);
    }

}

void Line_Follower::adjust(int direction)
{
    if (direction == LEFTTURN)
    {   
        Serial.print("in adjusting left block ");
        motorDrive(_correctionFactor * baseSpeed, baseSpeed); //adjusting left so decrease left wheel speed
    }

    else if (direction == RIGHTTURN)
    {
        Serial.print("in adjusting right block ");
        motorDrive(baseSpeed, _correctionFactor * baseSpeed); //adjusting right so decrease right wheel speed
    }
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
    Serial.print("turning left now");
    motorDrive(-turnSpeed, turnSpeed);

    delay(_turnDelay);
    _leftReading = digitalRead(LINESENSOR2);
    while(_leftReading != 1)
    {
        _leftReading = digitalRead(LINESENSOR2);   
    }
    delay(50);    
}

void Line_Follower::rightTurn()
{
    motorDrive(turnSpeed, -turnSpeed); 
    delay(_turnDelay);
    _rightReading = digitalRead(LINESENSOR3);
    while(_rightReading != 1)
    {
        _rightReading = digitalRead(LINESENSOR3);
    }
    delay(50);
}

void Line_Follower::straight(uint16_t duration)
{
    motorDrive(baseSpeed, baseSpeed);
    delay(duration);
}

void Line_Follower::turn180(int direction)
{      
    if (direction == LEFT)
    {
        motorDrive(-turnSpeed, turnSpeed);
        delay(_turnDelay);
        //_turnStart = millis();
        readAllLFSensors();
        while (_leftReading == 0)
        { //TIME TURN TO 90 DEGREES.
            readAllLFSensors();
        }
    }
    else
    {
        motorDrive(turnSpeed, -turnSpeed);
        delay(_turnDelay);
        //_turnStart = millis();
        readAllLFSensors();
        while (_rightReading == 0)
        { //TIME TURN TO 90 DEGREES.
            readAllLFSensors();
        }
    }
    
    //_turnMid = millis();
    //delay((_turnMid - _turnStart)); //CONTINUES TURN FOR SAME TIME IT TOOK TO REACH 90 DEGREES
    stop();
}

void Line_Follower::turn90(bool left, bool OnJunction)
{
    if (!OnJunction)
    {
        if (left)
        {
            motorDrive(-turnSpeed, turnSpeed);
            while (_extremeLeftReading == 0)
            { // TURN 90 DEGREES.
                readAllLFSensors();
            }
            //Straightening adjustment
        }
        else
        {
            motorDrive(turnSpeed, -turnSpeed);
            while (_extremeRightReading == 0)
            { // TURN 90 DEGREES.
                readAllLFSensors();
            }
            //Straightening adjustment
        }
        stop();
        return;
    }
    else
    {
        return;
    }
}

void Line_Follower::driveForwardApproachSpeed(unsigned long time_ms)
{

    unsigned long starttime = millis();    
    unsigned long endtime = starttime;
    while ((endtime - starttime) <= time_ms) // do this loop for up to 1000mS
    {   
        motorDrive(approachSpeed, approachSpeed);
        endtime = millis();
    }
    stop();
}

void Line_Follower::approachCube(uint32_t duration) //change duration to distance using ultrasound
{   
    Serial.print("in approach cube func ");
    //Currently just hard coding the distance from final turn to the block. Will likely need editing
    motorDrive(approachSpeed, approachSpeed);
    delay(200); //Clearing the white line to prevent junction detection when go() called
    baseSpeed = approachSpeed;
    for(uint32_t tStart = millis();  (millis()-tStart) < duration;) //Replace this with while ultrasound less than distance passed to func
    {
        go();
    }
    baseSpeed = 200;
    stop();
}

void Line_Follower::approachHome(float duration)
{
    motorDrive(baseSpeed, baseSpeed);
    delay(100); //Clearing the white line to prevent junction detection when go() called
    unsigned long starttime = millis();    
    unsigned long endtime = starttime;
    while ((endtime - starttime) <= duration) // do this loop for up to 1000mS
    {   
        go();
        endtime = millis();
    }
    stop();
}

void Line_Follower::junction()
{   
    Serial.print("in junction func");
    Serial.print(_currentRoute[pos]);
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
            straight(_continueDelay);
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
            Serial.print("in block section");
            stop();
            Serial.print("returned from stop");
            cubeRetrieval.prepare(); //7500 for commercial zone pickup, change to limit switch

            if (_blocksCollected == 0) approachCube(1000); //note at slow speed
            else if (_blocksCollected == 1) approachCube(1000);
            //Either more else ifs or potentially add new switch case for industrial blocks
            _blockHard =  cubeRetrieval.pickUp();
            delay(2000);
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
            //Maybe some code to drive forwards before turning around??
            turn180(LEFT);
            break;
        }
        
        case HOME:
        {
            approachHome(1000); //this distance will change to ultrasound reading to wall at dropoff position
            
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
                case 2:
                {
                    if (_blockHard) _currentRoute = Routes::CollectBlockSFromRed;
                    else _currentRoute = Routes::CollectBlockLFromGreen;
                    break;
                }
                case 3:
                {
                    if (_currentRoute == Routes::BringBlockLToGreen) _currentRoute = Routes::CollectBlockSFromGreen;
                    else if (_currentRoute == Routes::BringBlockLToRed) _currentRoute = Routes::CollectBlockSFromRed;
                    else if (_currentRoute == Routes::BringBlockSToGreen) _currentRoute = Routes::CollectBlockLFromGreen;
                    else if (_currentRoute == Routes::BringBlockSToRed) _currentRoute = Routes::CollectBlockLFromRed;
                }
                case 4:
                {
                    if (_blockHard) _currentRoute = Routes::ReturnHomeFromRed;
                    else _currentRoute = Routes::ReturnHomeFromGreen;
                }
                default:
                {
                    break;
                }
                
            }

            motorDrive(-baseSpeed, -baseSpeed);
            delay(_reverseTime);
            stop();

            cubeRetrieval.raiseClaw();
            if (_blockHard)
            {
                turn180(LEFT);
            }
            else turn180(RIGHT);
            
            pos = 0;
            break;
        }

        case ENTERSWAN:
        {   
            // drive forward until in front of factory
            while (TimeOfFlight.GetDistance() > swanDist)
            {
                go();
            }

            //time for something
            baseSpeed = 150;
            unsigned long starttime = millis();    
            unsigned long endtime = starttime;
            baseSpeed = 175;
            while ((endtime - starttime) <= 1500) // do this loop for up to 1000mS
            {   
                go();
                endtime = millis();
            }
            baseSpeed = 200;
            stop();
             // drive past start of factory for pre-defined ms to position

            // We are positioned in front of the gate now
            turn90(false, false);
            cubeRetrieval.prepare();
            driveForwardApproachSpeed(3500); // drive to cube and push up against back section (against blocker?)            // pickup block
            //Potential need to reverse small distance here
            stop();
            _blockHard = cubeRetrieval.pickUp();
            delay(2000);
            ledsOff();

            _blocksCollected++;
            //Select route home based on current array and blockHard
            if (_blockHard) _currentRoute = Routes::BringBlockSToRed;
            else _currentRoute = Routes::BringBlockSToGreen;
            pos = 0;

            // reverse until hit white line:
            while (_extremeLeftReading == 0 && _extremeRightReading == 0)
            {
                motorDrive(-baseSpeed, -baseSpeed);                
            }
            stop();

            // turn to right on line for now:
            if (_currentRoute == Routes::BringBlockSToRed) turn90(true, false);
            else turn90(false, false);
            break;
        }

        /*case EXITSWAN:
        {
            // reverse until hit white line:
            while (_extremeLeftReading == 0 && _extremeRightReading == 0)
            {
                motorDrive(-baseSpeed, -baseSpeed);                
            }
            motorDrive(0, 0);

            // turn to right on line for now:
            turn90(false, false);

            break;
        }*/

        case ENTERLUCOZADE:
        {
            // drive forward until in front of factory
            int LucozadeDist = 10;
            while (TimeOfFlight.GetDistance() > LucozadeDist)
            {
                go();
            }

            for(uint32_t tStart = millis();  (millis()-tStart) < 500;)
            {
                go();
            }

             // drive past start of factory for pre-defined ms to position
            stop();

            // We are positioned in front of the gate now
            turn90(false, false);
            driveForwardApproachSpeed(3000); // drive to cube by going forward 100ms

            // pickup block

            _blockHard =  cubeRetrieval.pickUp();
            ledsOff();

            _blocksCollected++;
            //Select route home based on current array and blockHard
            if (_blockHard)_currentRoute = Routes::BringBlockLToRed;
            else _currentRoute = Routes::BringBlockLToGreen;

            pos = 0;
            // reverse until hit white line:
            while (_extremeLeftReading == 0 && _extremeRightReading == 0)
            {
                motorDrive(-baseSpeed, -baseSpeed);
            }
            stop();
            turn90(true, false);
            break;
        }
        /*case EXITLUCOZADE:
        {
            // reverse until hit white line:
            while (_extremeLeftReading == 0 && _extremeRightReading == 0)
            {
                motorDrive(-baseSpeed, -baseSpeed);
            }
            stop();
            turn90(true, false);
        }*/

        default:
        {
            straight(200); //to clear junction for line line following
            break;
        }
    }
}

void Line_Follower::stop()
{
    _rightMotor -> run(RELEASE);
    _leftMotor -> run(RELEASE);
    Serial.print("stopping ");
}

void Line_Follower::ledsOff()
{
    digitalWrite(REDLED, LOW);
    digitalWrite(GREENLED, LOW);
}
