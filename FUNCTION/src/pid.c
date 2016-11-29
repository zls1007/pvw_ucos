#include "pid.h"

float PID_Fuc(PIDParam_TypeDef *param)
{
	float inc;  //定义增量

	//pid公式
	inc = param->Kp*(param->err - param->err_next) + param->Ki*param->err + param->Kd*(param->err - 2*param->err_next + param->err_last);
	
	//更新参数
	param->err_last = param->err_next;
	param->err_next = param->err;
	
	//返回增量值
	return inc;
}

