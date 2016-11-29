#ifndef __PRESSURE_DATA_TASK_H
#define __PRESSURE_DATA_TASK_H

#include "includes.h"

//任务优先级
#define PRESSURE_DATA_TASK_PRIO		8
//任务堆栈大小
#define PRESSURE_DATA_STK_SIZE		256

//任务函数
void pressureData_task(void *p_arg);
//任务创建函数
void pressureData_task_create(void);

#endif
