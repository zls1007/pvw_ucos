#ifndef __LED_TASK_H
#define __LED_TASK_H

#include "includes.h"

//任务优先级
#define LED_TASK_PRIO		4
//任务堆栈大小	
#define LED_STK_SIZE 		128

//任务函数
void led_task(void *p_arg);
//任务创建函数
void led_task_create(void);

#endif
