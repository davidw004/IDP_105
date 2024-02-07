#include "Line_Follower.h"

Line_Follower::Line_Follower()
{
    maxSpeedLeft = 150;
    maxSpeedRight = 150;
    baseSpeedLeft = 150;
    baseSpeedRight = 150;
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



    _leftMotor = AFMS.getMotor(1);
    _rightMotor = AFMS.getMotor(2);
    cubeRetrieval.setup();
    
    AFMS.begin();
    _leftMotor->setSpeed(baseSpeedLeft);
    _rightMotor->setSpeed(baseSpeedRight);
}

void Line_Follower::exitbox()
{

    //Turn LED on
    digitalWrite(BLUELED, HIGH);

    //Run motors forwards
    _leftMotor->run(BACKWARD); //motors are connected in reverse
    _rightMotor->run(BACKWARD); //motors are connected in reverse

    //Wait until exited box, ie central line sensors are past the white line
    delay(2000); //Fine tune duration
    _leftMotor->run(RELEASE);
    _rightMotor->run(RELEASE); 
}

void Line_Follower::go()
{

    _extremeLeftReading = digitalRead(LINESENSOR1);
    _leftReading = digitalRead(LINESENSOR2);
    _rightReading = digitalRead(LINESENSOR3);
    _extremeRightReading = digitalRead(LINESENSOR4);
    

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
    if (_leftReading == 1 && _rightReading == 1){
        
        _leftMotor -> setSpeed(baseSpeedLeft);
        _rightMotor -> setSpeed(baseSpeedRight);
        //Run motors forwards
        _leftMotor->run(BACKWARD); //motors are connected in reverse
        _rightMotor->run(BACKWARD); //motors are connected in reverse
    }
    else if (_leftReading == 1 && _rightReading == 0){ //If left high and right low then change motor speed to turn left
        _leftMotor -> setSpeed(0.8 * baseSpeedLeft);
        _rightMotor -> setSpeed(baseSpeedRight);
        //Run motors forwards
        _leftMotor->run(BACKWARD); //motors are connected in reverse
        _rightMotor->run(BACKWARD); //motors are connected in reverse
    }
    
    else if (_leftReading == 0 && _rightReading == 1){ //If left high and right low then change motor speed to turn left
        _leftMotor -> setSpeed(baseSpeedLeft);
        _rightMotor -> setSpeed(0.8 * baseSpeedRight);
        //Run motors forwards
        _leftMotor->run(BACKWARD); //motors are connected in reverse
        _rightMotor->run(BACKWARD); //motors are connected in reverse
    }
    else {
        _leftMotor -> run(RELEASE);
        _rightMotor -> run(RELEASE);
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
    _leftMotor -> run(RELEASE);
    _rightMotor -> run(RELEASE);
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
    _leftMotor -> run(RELEASE);
    _rightMotor -> run(RELEASE);
}

void Line_Follower::straight()
{
    _leftMotor -> setSpeed(baseSpeedLeft);
    _rightMotor -> setSpeed(baseSpeedRight);
    _leftMotor->run(FORWARD);  // Replace if needed
    _rightMotor->run(FORWARD);
    delay(continueDelay);
}

void Line_Follower::approachCube()
{
    //Currently just hard coding the distance from final turn to the block. Will likely need editing
    startTime == millis();
    while ((millis() - startTime) < 3000)
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
        {   
            approachCube();
            _leftMotor->run(RELEASE);
            _rightMotor->run(RELEASE);
            //Collect block (call function as friend function)
            blockHard =  cubeRetrieval.pickUp();
            if (blockHard){digitalWrite(REDLED, HIGH);}
            else {digitalWrite(GREENLED, HIGH);}

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
            break;
        }
        case HOME:
        {
            isReturningCube = true;
            // Continue driving until 3 seconds elapsed
            while (isReturningCube == true)
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