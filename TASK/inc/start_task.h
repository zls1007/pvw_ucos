#ifndef __START_TASK_H
#define __START_TASK_H

#include "includes.h"

//任务优先级
#define START_TASK_PRIO		3
//任务堆栈大小	
#define START_STK_SIZE 		512

//任务函数
void start_task(void *p_arg);
void start_task_create(void);

#endif

