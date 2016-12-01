#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f4xx_conf.h"

void tim1_init(void);  //pwm 3路输出  电机控制
void tim3_init(void);  //pwm 2路输出  照明控制


#endif
