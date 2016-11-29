#ifndef __IMU_DATA_TASK_H
#define __IMU_DATA_TASK_H

#include "includes.h"

//任务优先级
#define IMU_DATA_TASK_PRIO		7
//任务堆栈大小
#define IMU_DATA_STK_SIZE		256

//任务函数
void imuData_task(void *p_arg);
//任务创建函数
void imuData_task_create(void);

#endif
