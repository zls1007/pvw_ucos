#ifndef __USART_DEAL_TASK_H
#define __USART_DEAL_TASK_H

#include "includes.h"

//�������ȼ�
#define USART_DEAL_TASK_PRIO		5
//�����ջ��С	
#define USART_DEAL_STK_SIZE       128

//������
void usartDeal_task(void *p_arg);
//���񴴽�����
void usartDeal_task_create(void);



#endif
