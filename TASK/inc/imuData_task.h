#ifndef __IMU_DATA_TASK_H
#define __IMU_DATA_TASK_H

#include "includes.h"

//�������ȼ�
#define IMU_DATA_TASK_PRIO		7
//�����ջ��С
#define IMU_DATA_STK_SIZE		256

//������
void imuData_task(void *p_arg);
//���񴴽�����
void imuData_task_create(void);

#endif
