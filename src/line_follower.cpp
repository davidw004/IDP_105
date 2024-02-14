#include "Line_Follower.h"
#include <Ticker.h>

bool wheel_running = false;    
bool blue_led_on = false;

extern Ticker tickerObject;

//Non-member function used to run delays without turing off blue LED
void TickerDelay(int ms)
{
    unsigned long start = millis();
    unsigned long end = start;
    while (end - start < ms)
    {
        tickerObject.update();
        end = millis();
    }
}

//Define all member variables
Line_Follower::Line_Follower()
{
    //Speeds
    maxSpeed = 255;
    baseSpeed = 200;
    turnSpeed = 150;
    approachSpeed = 125;
    approachIndustrialSpeed = 175;
    baseSweepSpeed = 75;

    _correctionFactor = 0.1;
    _continueDelay = 200;
    _turnDelay = 750;
    _correctionTime = 20;
    _reverseTime = 350;
    _exitBoxTime = 200;
    _timeFactor = 0.25;
    _commercialPrepare = 7500;
    appoachHomeDuration = 1000;
    approachSwanDuration = 2450;
    approachLucozadeDuration = 2000;
    swanDist = 30.0f;
    lucozadeDist = 30.0f;

    _currentRoute = Routes::CollectBlockOne; //changed to test
    _blocksCollected = 0; //edited
    pos = 0;
}

//Setup pins as I/O ports and setup motors
void Line_Follower::setup()
{
    //Setup line sensors
    pinMode(LINESENSOR1, INPUT);
    pinMode(LINESENSOR2, INPUT);
    pinMode(LINESENSOR3, INPUT);
    pinMode(LINESENSOR4, INPUT);
    pinMode(BUTTON1, INPUT);

    //Setup LEDs
    pinMode(REDLED, OUTPUT);
    pinMode(GREENLED, OUTPUT);
    pinMode(BLUELED, OUTPUT);

    //Setup motors and cube retrieval / TOF 
    _leftMotor = AFMS.getMotor(1);
    _rightMotor = AFMS.getMotor(2);
    cubeRetrieval.setup();
    TimeOfFlight.Setup();
}

//Function to read all line sensors and set readings
void Line_Follower::readAllLFSensors()
{
    _extremeLeftReading = digitalRead(LINESENSOR1);
    _leftReading = digitalRead(LINESENSOR2);
    _rightReading = digitalRead(LINESENSOR3);
    _extremeRightReading = digitalRead(LINESENSOR4); 
}

//Currently unused as robot never comes off track
void Line_Follower::sweep() 
{
    //FINDS LINE. ONLY WORKS IF LINE IS BETWEEN INNER AND OUTER SENSORS, AND AWAY FROM A JUNCTION 
    motorDrive(-baseSweepSpeed, baseSweepSpeed); //sweeps left 
    _startTime = millis();
    for(uint32_t _startTime = millis();  (millis() - _startTime) < 1000;) //Replace this with while ultrasound less than distance passed to func
    {
        readAllLFSensors();
        if (_extremeRightReading == 1 ){ //line to the right of the robot 
            while(digitalRead(LINESENSOR3) == 0){
                tickerObject.update();
                _leftMotor -> run(BACKWARD);
                _rightMotor -> setSpeed(0.3 * baseSweepSpeed);
            }
        }
        else if (_extremeLeftReading == 1){ //line to left of robot
            while(digitalRead(LINESENSOR2) == 0) { //drive torwards line until hit. 
                _leftMotor -> run(BACKWARD);
                _leftMotor -> setSpeed(baseSweepSpeed);
                tickerObject.update();
            }
        }
    }
 
}

//Code to exit box
void Line_Follower::exitbox() 
{
    cubeRetrieval.raiseClaw(); //Raise claw first
    while (_extremeLeftReading == 0 && _extremeRightReading == 0) //Drive up to box edge
    {
        tickerObject.update();
        motorDrive(baseSpeed, baseSpeed);
        readAllLFSensors();
        Serial.print("exiting box ");
    }
    Serial.print("exited box and waiting ");
    TickerDelay(_exitBoxTime); //Wait until exited box, ie central line sensors are past the white line
}

//Test function used for industrial area
void Line_Follower::swanTest() 
{
    cubeRetrieval.raiseClaw();
    //Turn LED on
    _currentRoute = Routes::TestSwan;
}

//Runs motors at the speeds entered
void Line_Follower::motorDrive(int lspeed, int rspeed) 
{
    if ( lspeed != 0 || rspeed != 0) wheel_running = true; //Indicates robot is moving for ticker
    _leftMotor -> setSpeed(abs(lspeed));
    _rightMotor -> setSpeed(abs(rspeed));

    if (lspeed < 0){_leftMotor -> run(FORWARD);} //If negative run motor in reverse
    else if (lspeed > 0){_leftMotor -> run(BACKWARD);} //If positive run correct direction
    else if (lspeed == 0){_leftMotor -> run(RELEASE);} //If 0 stop running motor

    if (rspeed < 0){_rightMotor -> run(FORWARD);}
    else if (rspeed > 0){_rightMotor -> run(BACKWARD);}
    else if (rspeed == 0){_rightMotor -> run(RELEASE);}
}

//Looped function which line follows
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

    else if (_leftReading == 1 && _rightReading == 0)
    { //If left high and right low then change motor speed to turn left
        Serial.print("adjusting left ");
        adjust(LEFTTURN);
    }

    else if (_leftReading == 0 && _rightReading == 1)
    { //If left low and right high then change motor speed to turn right
        Serial.print("adjusting right ");
        adjust(RIGHTTURN);
    }

    else if (_leftReading == 0 && _rightReading == 0)
    {
        motorDrive(baseSpeed, baseSpeed);
    }

}

//Run go for duration passed into func at speed given
void Line_Follower::timedGo(int duration, int speed) 
{
    baseSpeed = speed; //This is very questionable but too much effort to change code since currently working
    unsigned long starttime = millis();    
    unsigned long endtime = starttime;
    while ((endtime - starttime) <= duration) // do this loop for duration
    {   
        go();
        tickerObject.update();
        endtime = millis();
    }
    baseSpeed = 200; //Set baseSpeed back to what it should be
}

//Line follower extension
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

//Turn left until left sensor is on line
void Line_Follower::leftTurn() 
{   
    Serial.print("turning left now");
    motorDrive(-turnSpeed, turnSpeed);

    TickerDelay(_turnDelay);
    _leftReading = digitalRead(LINESENSOR2);
    while(_leftReading != 1)
    {
        tickerObject.update();
        _leftReading = digitalRead(LINESENSOR2);   
    }
    TickerDelay(50); //Small delay to allow wheels to reach central position
}

//Turn right until right sensor is on line
void Line_Follower::rightTurn() 
{
    motorDrive(turnSpeed, -turnSpeed); 
    TickerDelay(_turnDelay);
    _rightReading = digitalRead(LINESENSOR3);
    while(_rightReading != 1)
    {
        tickerObject.update();
        _rightReading = digitalRead(LINESENSOR3);
    }
    TickerDelay(50); ////Small delay to allow wheels to reach central position
}

//Drive straight for duration passed into func
void Line_Follower::straight(uint16_t duration, int speed) 
{
    motorDrive(speed, speed);
    TickerDelay(duration);
}

//Reverse at speed given for duration
void Line_Follower::reverse(int duration, int speed) 
{
    unsigned long starttime = millis();    
    unsigned long endtime = starttime;
    while ((endtime - starttime) <= duration)
    {   
        motorDrive(-speed, -speed);
        tickerObject.update();
        endtime = millis();
    }
    stop(); //Will always stop motors after reversing
}

//Turn 180 on line in either left or right direction
void Line_Follower::turn180(int direction) 
{      
    if (direction == LEFTTURN)
    {
        motorDrive(-turnSpeed, turnSpeed);
        TickerDelay(_turnDelay);
        //_turnStart = millis();
        readAllLFSensors();
        while (_rightReading == 0)
        { //TIME TURN TO 90 DEGREES.
            tickerObject.update();
            readAllLFSensors();
        }
    }
    else
    {
        motorDrive(turnSpeed, -turnSpeed);
        TickerDelay(_turnDelay);
        //_turnStart = millis();
        readAllLFSensors();
        while (_leftReading == 0)
        { //TIME TURN TO 90 DEGREES.
            tickerObject.update();
            readAllLFSensors();
        }
    }
    
    //_turnMid = millis();
    //TickerDelay((_turnMid - _turnStart)); //CONTINUES TURN FOR SAME TIME IT TOOK TO REACH 90 DEGREES
    stop();
}

//When turning into factory, decide whether wide or narrow sensors trigger
void Line_Follower::turnFactory(int direction, bool leaving) 
{
    if (!leaving)
    {
        if (direction == LEFTTURN) //Currently redundant as will always turn right into empty regions
        {   
            motorDrive(-turnSpeed, turnSpeed);
            TickerDelay(_turnDelay); //Delay to clear line for detection (not actually needed for wide sensor)
            while (_extremeRightReading == 0) {tickerObject.update(); readAllLFSensors();}
        }
        else
        {
            motorDrive(turnSpeed, -turnSpeed);
            TickerDelay(_turnDelay); //Delay to clear line for detection (not actually needed for wide sensor)
            while (_extremeLeftReading == 0) {tickerObject.update(); readAllLFSensors();}
        }
        TickerDelay(250); //Small delay to straighten up
        stop();
    }
    else
    {
        if (direction == LEFTTURN) {leftTurn(); stop();} //WHen leaving can use normal left turn
        else rightTurn(); stop();
    }
}

//change duration to distance using ultrasound
void Line_Follower::approachCube(uint32_t duration) 
{   
    //Currently just hard coding the distance from final turn to the block. Will likely need editing
    straight(400, approachSpeed); //Clearing the white line to prevent junction detection when go() called
    timedGo(duration, approachSpeed);
    stop();
}

//Hard coded duration to drive to home from final junction
void Line_Follower::approachHome(float duration) 
{
    motorDrive(baseSpeed, baseSpeed);
    TickerDelay(100); //Clearing the white line to prevent junction detection when go() called
    timedGo(duration, baseSpeed);
    stop(); //Will always stop after approaching home
}

//From last junction, run this to collect cube from industrial
void Line_Follower::enterIndustrial(int duration, int durationPush) 
{
    // drive forward until in front of factory
    while (TimeOfFlight.GetDistance() > swanDist)
    {
        go();
        tickerObject.update();
    }
    stop();
    timedGo(duration, approachIndustrialSpeed); //Carry on driving forwards to reach centre of open section
    stop();
        // drive past start of factory for pre-defined ms to position

    // We are positioned in front of the gate now, turn right with leaving false (ie entering)
    turnFactory(RIGHTTURN, false);
    reverse(300, approachSpeed);
    cubeRetrieval.prepare();
    straight(durationPush, approachSpeed); // drive to cube and push up against back section (against blocker?)
    //Potential need to reverse small distance here
    stop();
    reverse(200, approachSpeed);
    cubeRetrieval.closeClaw(4000); //Close claw so ultrasound can detect cube type

    TickerDelay(1000);
    _blockHard = cubeRetrieval.detectCube();
    cubeRetrieval.raiseClaw();
    TickerDelay(2000);

    ledsOff();
}

//Reverse until wide line sensors triggered
void Line_Follower::reverseToLine() 
{
    readAllLFSensors();
    while (_extremeLeftReading == 0 && _extremeRightReading == 0)
    {
        motorDrive(-baseSpeed, -baseSpeed);
        tickerObject.update();
        readAllLFSensors();
    }
    stop();
}

//Decision on where to turn then block related functions
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
            straight(_continueDelay, baseSpeed);
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
            
            if (_blocksCollected == 0) 
            {   
                cubeRetrieval.prepare();
                approachCube(1250); //note at slow speed
            }
            else if (_blocksCollected == 1)
            {   
                reverse(200, approachSpeed);
                cubeRetrieval.prepare();
                approachCube(1200);
            }

            _blockHard =  cubeRetrieval.detectCube();
            reverse(100, approachSpeed);
            cubeRetrieval.raiseClaw();
            //TickerDelay(2000);
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
            turn180(LEFTTURN);
            break;
        }
        
        case HOME:
        {
            approachHome(appoachHomeDuration); //drive for set duration of time before dropping off cube
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
                    break;
                }

                case 4:
                {
                    if (_blockHard) _currentRoute = Routes::ReturnHomeFromRed;
                    else _currentRoute = Routes::ReturnHomeFromGreen;
                    break;
                }

                default:
                {
                    break;
                }
            }

            motorDrive(-baseSpeed, -baseSpeed); //Reverse to bring claw up
            TickerDelay(_reverseTime);
            stop();

            cubeRetrieval.raiseClaw(); //Lift claw

            if (_blockHard) //Depending on which side, turn 180
            {
                turn180(LEFTTURN);
            }
            else turn180(RIGHTTURN);
            pos = 0;
            break;
        }

        case ENTERSWAN:
        {   
            enterIndustrial(approachSwanDuration, 3550);
            _blocksCollected++;
            //Select route home based on current array and blockHard
            if (_blockHard) _currentRoute = Routes::BringBlockSToRed;
            else _currentRoute = Routes::BringBlockSToGreen;
            pos = 0;
            reverseToLine();

            // turn to right on line for now:
            if (_currentRoute == Routes::BringBlockSToRed) {turnFactory(LEFTTURN, true); }
            else { turnFactory(RIGHTTURN, true); }
            break;
        }

        case ENTERLUCOZADE:
        {
            enterIndustrial(approachLucozadeDuration, 3750);
            _blocksCollected++;
            //Select route home based on current array and blockHard
            if (_blockHard) _currentRoute = Routes::BringBlockLToRed;
            else _currentRoute = Routes::BringBlockLToGreen;
            pos = 0;
            // reverse until hit white line:
            reverseToLine();
            turnFactory(LEFTTURN, true);
            break;
        }

        case STARTBOX:
        {
            timedGo(1000, baseSpeed);
            readAllLFSensors();
            while (_extremeLeftReading == 0 && _extremeRightReading == 0)
            {
                go();
                readAllLFSensors();
            }
            motorDrive(approachSpeed, approachSpeed);
            TickerDelay(1550);
            while(true) stop();
            break;
        }
        default:
        {
            straight(_continueDelay, baseSpeed); //to clear junction for line line following
            break;
        }
    }
}

//Stop all motors (can techically be called through motorDrive but more obvious here)
void Line_Follower::stop() 
{
    _rightMotor -> run(RELEASE);
    _leftMotor -> run(RELEASE);
    wheel_running = false;
    Serial.print("stopping ");
}

//Turn detection LEDs off
void Line_Follower::ledsOff() 
{
    digitalWrite(REDLED, LOW);
    digitalWrite(GREENLED, LOW);
}
