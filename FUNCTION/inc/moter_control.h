#ifndef __MOVE_CONTROL_H
#define __MOVE_CONTROL_H
#include "stm32f4xx.h"
#include "includes.h"
#include "system_param.h"


void manualControlFuc(MoveMode_Type myLastState);
void pitchControlFuc(MoveMode_Type myLastState);
void deepControlFuc(MoveMode_Type myLastState);
void dirControlFuc(MoveMode_Type myLastState);
void allControlFuc(MoveMode_Type myLastState);

void MoterPWMoutput(s16 p1, s16 p2, s16 p3);

#endif
