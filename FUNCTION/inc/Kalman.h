#ifndef __KALMAN_H
#define __KALMAN_H

#include "stm32f4xx_conf.h"

void Kalman_Filter(int16_t imu[], int offset_gyro[]);

#endif
