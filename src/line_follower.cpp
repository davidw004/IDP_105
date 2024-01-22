#include "Arduino.h"
#include "Line_Follower.h"
Line_Follower::Line_Follower(uint8_t PWM_R_EN, uint8_t PWM_L_EN, uint8_t MAX_PWM_SPEED)
{
    _PWM_R_EN = PWM_R_EN;
    _PWM_L_EN = PWM_L_EN;
    _MAX_PWM_SPEED = (255 * MAX_PWM_SPEED) / 100;
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    pinMode(_PWM_R_EN, OUTPUT);
    pinMode(_PWM_L_EN, OUTPUT);
}

void Line_Follower::go() {
    //Read input from sensors (not yet figured out what values are being returned yet)
    _A0 = map(analogRead(A0), 0, 850, 0, (_MAX_PWM_SPEED * 0.7));
    _A1 = map(analogRead(A1), 0, 850, 0, (_MAX_PWM_SPEED * 0.3));
    _A2 = map(analogRead(A2), 0, 850, 0, (_MAX_PWM_SPEED * 0.3));
    _A3 = map(analogRead(A3), 0, 850, 0, (_MAX_PWM_SPEED * 0.7));
    
    //If both A1 A2 black keep driving at maxspeed
    if (_A1 == 0 && _A2 == 0){
        analogWrite(_PWM_L_EN, baseSpeedLeft);
        analogWrite(_PWM_R_EN, baseSpeedRight);
    }
    else if (_A1 == 1 && _A2 == 0){ //If left high and right low then change motor speed to turn left

    }

    /*
    if (_A_D_Scan == LOW)   // Analog Scan = LOW  
    {
        _A0 = map(analogRead(A0), 0, 850, 0, (_MAX_PWM_SPEED * 0.7));
        _A1 = map(analogRead(A1), 0, 850, 0, (_MAX_PWM_SPEED * 0.3));
        _A2 = map(analogRead(A2), 0, 850, 0, (_MAX_PWM_SPEED * 0.3));
        _A3 = map(analogRead(A3), 0, 850, 0, (_MAX_PWM_SPEED * 0.7));

        _left_weight = _A0 + _A1;
        _right_weight = _A3 + _A4;
        _left_weight = constrain(_left_weight, 0, _MAX_PWM_SPEED);
        _right_weight = constrain(_right_weight, 0, _MAX_PWM_SPEED);

        // for sharp corners 
        if (_left_weight > (_MAX_PWM_SPEED * 0.8)) _right_weight = _right_weight * 0.3;
        if (_right_weight > (_MAX_PWM_SPEED * 0.8)) _left_weight = _left_weight * 0.3;

        if (_LineColor == LOW) // Black Line =LOW 
        {
            analogWrite(_PWM_R_EN, (_MAX_PWM_SPEED - _right_weight));
            analogWrite(_PWM_L_EN, (_MAX_PWM_SPEED - _left_weight));
        }
        else                // White Line =HIGH;
        {
            analogWrite(_PWM_R_EN, _right_weight);
            analogWrite(_PWM_L_EN, _left_weight);
        }
    }
    else                     // Digital Scan = HIGH;
    {
        _Position = 0;
        bitWrite(_Position, 0, digitalRead(A0));
        bitWrite(_Position, 1, digitalRead(A1));
        bitWrite(_Position, 2, digitalRead(A2));
        bitWrite(_Position, 3, digitalRead(A3));
        bitWrite(_Position, 4, digitalRead(A4));
        if (_LineColor == HIGH)           // Black Line =LOW , White Line =HIGH;
        {
            _Position = ~_Position;
            _Position = _Position & 31;
        }

        PORTB = _Position;
        switch (_Position)
        {
        case 4:  //00100
            analogWrite(_PWM_R_EN, _MAX_PWM_SPEED);
            analogWrite(_PWM_L_EN, _MAX_PWM_SPEED);
            break;
        case 12: //01100 drift Right
            analogWrite(_PWM_R_EN, (_MAX_PWM_SPEED * 4 / 5));
            analogWrite(_PWM_L_EN, _MAX_PWM_SPEED);
            break;
        case 8: // 01000 more Right
            analogWrite(_PWM_R_EN, (_MAX_PWM_SPEED * 3 / 5));
            analogWrite(_PWM_L_EN, _MAX_PWM_SPEED);
            break;
        case 24: //11000 extra Right
            analogWrite(_PWM_R_EN, (_MAX_PWM_SPEED * 2 / 5));
            analogWrite(_PWM_L_EN, _MAX_PWM_SPEED);
            break;
        case 16: //10000 sharp Right
            analogWrite(_PWM_R_EN, 0);
            analogWrite(_PWM_L_EN, _MAX_PWM_SPEED);
            break;
        case 28: //11100 sharp Right
            analogWrite(_PWM_R_EN, 0);
            analogWrite(_PWM_L_EN, _MAX_PWM_SPEED);
            break;
        case 30: //11110 sharp Right
            analogWrite(_PWM_R_EN, 0);
            analogWrite(_PWM_L_EN, _MAX_PWM_SPEED);
            break;
        case 6:  //00110 drift Left
            analogWrite(_PWM_R_EN, _MAX_PWM_SPEED);
            analogWrite(_PWM_L_EN, (_MAX_PWM_SPEED * 4 / 5));
            break;
        case 2: // 00010 more Left
            analogWrite(_PWM_R_EN, _MAX_PWM_SPEED);
            analogWrite(_PWM_L_EN, (_MAX_PWM_SPEED * 3 / 5));
            break;
        case 3: // 00011 extra Left
            analogWrite(_PWM_R_EN, _MAX_PWM_SPEED);
            analogWrite(_PWM_L_EN, (_MAX_PWM_SPEED * 2 / 5));
            break;
        case 1: // 00001 sharp Left
            analogWrite(_PWM_R_EN, _MAX_PWM_SPEED);
            analogWrite(_PWM_L_EN, 0);
            break;
        case 7: // 00111 sharp Left
            analogWrite(_PWM_R_EN, _MAX_PWM_SPEED);
            analogWrite(_PWM_L_EN, 0);
            break;
        case 15: // 01111 sharp Left
            analogWrite(_PWM_R_EN, _MAX_PWM_SPEED);
            analogWrite(_PWM_L_EN, 0);
            break;
        default:
            analogWrite(_PWM_R_EN, 0);
            analogWrite(_PWM_L_EN, 0);
            break;
        }
    }
    */
}
void Line_Follower::stop() {
    analogWrite(_PWM_R_EN, 0);
    analogWrite(_PWM_L_EN, 0);
}