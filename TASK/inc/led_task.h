#ifndef __LED_TASK_H
#define __LED_TASK_H

#include "includes.h"

//�������ȼ�
#define LED_TASK_PRIO		4
//�����ջ��С	
#define LED_STK_SIZE 		128

//������
void led_task(void *p_arg);
//���񴴽�����
void led_task_create(void);

#endif
