#ifndef __PRESSURE_DATA_TASK_H
#define __PRESSURE_DATA_TASK_H

#include "includes.h"

//�������ȼ�
#define PRESSURE_DATA_TASK_PRIO		8
//�����ջ��С
#define PRESSURE_DATA_STK_SIZE		256

//������
void pressureData_task(void *p_arg);
//���񴴽�����
void pressureData_task_create(void);

#endif
