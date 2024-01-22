#pragma once
#include "Arduino.h"
class Line_Follower {
public:
    Line_Follower(bool LineColor, bool A_D_Scan, uint8_t PWM_R_EN, uint8_t PWM_L_EN, uint8_t MAX_PWM_SPEED); // :
    void  Go();
    void  Stop();
    void leftTurn();
private:
    bool _LineColor;
    bool _A_D_Scan;
    uint8_t _Position;
    uint8_t _A0, _A1, _A2, _A3, _A4;
    uint8_t _R_Speed, _L_Speed;
    uint8_t _PWM_R_EN, _PWM_L_EN;
    uint8_t _MAX_PWM_SPEED;
    uint8_t _left_wieght;
    uint8_t _right_wieght;
    uint8_t maxspeeda = 150;
    uint8_t maxspeedb = 150;
    uint8_t basespeeda = 100;
    uint8_t basespeedb = 100;
};