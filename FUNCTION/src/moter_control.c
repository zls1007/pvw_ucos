#include "moter_control.h"
#include "param.h"
#include "pid.h"

//开环控制函数
void manualControlFuc(MoveMode_Type myLastState)
{
	s16 x, r, z;  			//用户输入运动指令  -100 ~ +100
	s16 p1, p2, p3; 		//电机速度值  -500 ~ 500
	
	//获取用户输入
	GetMoveSpeed(&x, &r, &z);
	
//	//去除死区
//	if(x > 10) x -= 10;
//	else if(x < -10) x += 10;
//	else x = 0;
//	if(r > 15) r -= 15;
//	else if(r < -15) r += 15;
//	else r = 0;
//	if(z > 10) z -= 10;
//	else if(z < -10) z += 10;
//	else z = 0;
	
	//速度映射
		p1 = x*5 + r*3;
		p2 = -x*5 + r*3;
		p3 = z*3;
		
	//输出控制
	MoterPWMoutput(p1, p2, p3);
}


//俯仰姿态控制
void pitchControlFuc(MoveMode_Type myLastState)
{
	s16 x, r, z;  						//用户输入运动指令  -100 ~ +100
	s16 p1, p2, p3; 					//电机速度值  -500 ~ 500
	float roll, pitch, yaw; 		//机器人姿态  *100
	static int p3_inc;								//姿态导致垂直电机转动
	static PIDParam_TypeDef myparam; //PID参数
	
	//获取用户输入
	GetMoveSpeed(&x, &r, &z);
	//获取机器人姿态
	GetAttitudeData(&roll, &pitch, &yaw);
	//获取PID参数
	GetPitchPID(&myparam.Kp, &myparam.Ki, &myparam.Kd);
	
	//计算偏差
	myparam.err = 0 - pitch;
	//PID算法
	p3_inc += PID_Fuc(&myparam);
	
	//确定两侧电机输出
	p1 = x*5 + r*3;
	p2 = -x*5 + r*3;
	
	if(z == 0)  //定深
	{
		p3 = p3_inc;
	}else
	{
		p3 = z*3;
	}
	//printf("err=%d,kp=%d,ki=%d,kd=%d,inc=%d\r\n",myparam.err, myparam.Kp, myparam.Ki, myparam.Kd, p3_inc);
	//输出控制
	MoterPWMoutput(p1, p2, p3);
		
}

//定深运动模式
void deepControlFuc(MoveMode_Type myLastState)
{
	s16 x, r, z;  						//用户输入运动指令  -100 ~ +100
	s16 p1, p2, p3; 					//电机速度值  -500 ~ 500
	float roll, pitch, yaw; 		//机器人姿态  
	float deep;									//机器人深度  
	
	static float deep_origin;   		//目标深度	
	static float pitch_out;					//深度导致姿态变化
	static float p3_out;						//姿态导致垂直电机转动
	static float last_deep;
		
	static PIDParam_TypeDef pitch_param; //PID参数
	static PIDParam_TypeDef deep_param; //PID参数
	
	static u16 cnt = 0;
	
	
	//获取用户输入
	GetMoveSpeed(&x, &r, &z);
	//获取机器人姿态
	GetAttitudeData(&roll, &pitch, &yaw);
	//获取机器人深度
	GetDeepData(&deep);
	
	//判断是否为第一次进入该模式
	if(myLastState != MODE_DEEP)
	{
		deep_origin = deep;    //以当前深度为目标值
	}
//	if(((deep-last_deep) < 0.06)  && ((last_deep-deep) < 0.06) )  deep = last_deep;
//	else last_deep = deep;
	//printf("deep=%.2f,last=%.2f\r\n", deep, last_deep);
	
	//获取PID参数
	GetPitchPID(&pitch_param.Kp, &pitch_param.Ki, &pitch_param.Kd);
	GetDeepPID(&deep_param.Kp, &deep_param.Ki, &deep_param.Kd);
	
	//计算深度偏差   -+0.06 为允许静态误差（深度）
	deep_param.err = deep - deep_origin;
//	if(deep_param.err  > 0.06) deep_param.err  -= 0.06;
//	else if(deep_param.err  < -0.06) deep_param.err  += 0.06;
//	else deep_param.err  = 0.0;
	
	//分频 100Hz/5 = 20Hz
	if(cnt < 5) cnt ++;
	else
	{
		cnt = 0;
			//深度PID算法
		pitch_out += PID_Fuc(&deep_param);
	}

	
	
	//计算姿态偏差   -+1.0  为允许静态误差（俯仰）
	pitch_param.err  = pitch_out - pitch;
//	if(pitch_param.err  > 1.0) pitch_param.err  -= 1.0;
//	else if(pitch_param.err  < -1.0) pitch_param.err  += 1.0;
//	else pitch_param.err  = 0.0;

	
	//姿态PID算法
	p3_out += PID_Fuc(&pitch_param);

	
	//确定两侧电机输出
	p1 = x*5 + r*3;
	p2 = -x*5 + r*3;
	
	if(z == 0)  //定深
	{
		p3 = (s16)(p3_out);
	}else			
	{
		p3 = z*3;
		pitch_out = 0;
		deep_origin = deep;  //更新目标深度
	}
	
	//printf("d=%.2f,de=%.2f,po=%.2f,p=%.2f,pe=%.2f,inc=%.2f\r\n",deep, deep_param.err, pitch_out, pitch,pitch_param.err, p3_out);
	//printf("err=%d,kp=%d,ki=%d,kd=%d,inc=%d\r\n",myparam.err, myparam.Kp, myparam.Ki, myparam.Kd, p3_inc);
	//输出控制
	MoterPWMoutput(p1, p2, p3);
		
}


//定向运动模式
void dirControlFuc(MoveMode_Type myLastState)
{

}

//全闭环控制模式
void allControlFuc(MoveMode_Type myLastState)
{

}

//电机pwm输出
void MoterPWMoutput(s16 p1, s16 p2, s16 p3)
{
		s16 c1, c2, c3;  //无刷电机占空比 1000 ~ 2000
		static float cur_c3;
	
		//输入死区
		//if(abs(p1) < 60) p1 = 0;
		//if(abs(p2) < 30) p2 = 0;
	  //if(abs(p3) < 50) p3 = 0;
	  cur_c3 = 0.05*p3 + 0.95*cur_c3;
		p3 = (s16)cur_c3;
		if(abs(p3) < 40) p3 = 0;
		
		//占空比映射
		if(p1 > 0) c1 = 1542 + p1;
		else if(p1 < 0) c1 = 1498 + p1;
		else c1 = 1522;
		if(p2 > 0) c2 = 1542 + p2;
		else if(p2 < 0) c2 = 1498 + p2;
		else c2 = 1522;
		if(p3 > 0) c3 = 1540 + p3;
		else if(p3 < 0) c3 = 1496 + p3;
		else c3 = 1522;
//	c3 = 1522 + p3;
		
		//范围限定
		if(c1 > 2000) c1 = 2000; 
		else if(c1 < 1000) c1 = 1000;
		if(c2 > 2000) c2 = 2000; 
		else if(c2 < 1000) c2 = 1000;
		if(c3 > 2000) c3 = 2000; 
		else if(c3 < 1000) c3 = 1000;
		
		//printf("move:c1=%d, c2=%d, c3=%d\r\n", c1, c2, c3);
		
		//输出
		TIM_SetCompare4(TIM1, c1);
		TIM_SetCompare3(TIM1, c2);
		TIM_SetCompare2(TIM1, c3);
}
