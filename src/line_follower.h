#pragma once
#include "Arduino.h"
class Line_Follower {
public:
    Line_Follower(uint8_t PWM_R_EN, uint8_t PWM_L_EN, uint8_t MAX_PWM_SPEED); // :
    void go();
    void stop();

private:
    uint8_t _Position;
    uint8_t _A0, _A1, _A2, _A3;
    uint8_t _R_Speed, _L_Speed;
    uint8_t _PWM_R_EN, _PWM_L_EN;
    uint8_t _MAX_PWM_SPEED;
    uint8_t _left_weight;
    uint8_t _right_weight;
    uint8_t maxSpeedLeft = 150;
    uint8_t maxSpeedRight = 150;
    uint8_t baseSpeedLeft = 100;
    uint8_t baseSpeedRight = 100;
};