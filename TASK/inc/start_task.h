#ifndef __START_TASK_H
#define __START_TASK_H

#include "includes.h"

//�������ȼ�
#define START_TASK_PRIO		3
//�����ջ��С	
#define START_STK_SIZE 		512

//������
void start_task(void *p_arg);
void start_task_create(void);

#endif

