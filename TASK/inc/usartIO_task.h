#ifndef __USART_IO_TASK_H
#define __USART_IO_TASK_H

#include "includes.h"

//�������ȼ�
#define USART_IO_TASK_PRIO			6
//�����ջ��С	
#define USART_IO_STK_SIZE       256

//������
void usartIO_task(void *p_arg);
//���񴴽�����
void usartIO_task_create(void);



#endif
