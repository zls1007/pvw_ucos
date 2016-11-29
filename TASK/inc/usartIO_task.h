#ifndef __USART_IO_TASK_H
#define __USART_IO_TASK_H

#include "includes.h"

//任务优先级
#define USART_IO_TASK_PRIO			6
//任务堆栈大小	
#define USART_IO_STK_SIZE       256

//任务函数
void usartIO_task(void *p_arg);
//任务创建函数
void usartIO_task_create(void);



#endif
