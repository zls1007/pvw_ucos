#ifndef __PID_H
#define __PID_H

//����pid�����ṹ��
typedef struct _pid{
	float err; 						//����ƫ��
	float err_next;				//������һ�ε�ƫ��
	float err_last;				//�������ϴε�ƫ��
	float Kp, Ki, Kd;			//������������֡�΢��ϵ��
}PIDParam_TypeDef;

float PID_Fuc(PIDParam_TypeDef *param);

#endif

