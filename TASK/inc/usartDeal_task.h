#ifndef __USART_DEAL_TASK_H
#define __USART_DEAL_TASK_H

#include "includes.h"

//任务优先级
#define USART_DEAL_TASK_PRIO		5
//任务堆栈大小	
#define USART_DEAL_STK_SIZE       128

//任务函数
void usartDeal_task(void *p_arg);
//任务创建函数
void usartDeal_task_create(void);



#endif
