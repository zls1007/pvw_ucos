#ifndef __PID_H
#define __PID_H

//定义pid参数结构体
typedef struct _pid{
	float err; 						//定义偏差
	float err_next;				//定义上一次的偏差
	float err_last;				//定义再上次的偏差
	float Kp, Ki, Kd;			//定义比例、积分、微分系数
}PIDParam_TypeDef;

float PID_Fuc(PIDParam_TypeDef *param);

#endif

