#include "pid.h"

float PID_Fuc(PIDParam_TypeDef *param)
{
	float inc;  //��������

	//pid��ʽ
	inc = param->Kp*(param->err - param->err_next) + param->Ki*param->err + param->Kd*(param->err - 2*param->err_next + param->err_last);
	
	//���²���
	param->err_last = param->err_next;
	param->err_next = param->err;
	
	//��������ֵ
	return inc;
}

