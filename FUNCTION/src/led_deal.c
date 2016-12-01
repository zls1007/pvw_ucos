#include "led_deal.h"
//0.1s

//照明灯控制
void LedLightCtr(void)
{
	SysState_Type mState;   			//系统工作状态
	s16 d1, d2;										//用户输入灯光亮度
	
	static u8 cnt = 0;						//照明计数值
	static u16 ln, perio;  				//亮的周期和长度
	static SysState_Type lastState = MODE_MOVE; //记录上一次的系统工作状态
	
	//读取数值
	GetSysState(&mState);
	GetLedLight(&d1, &d2);
	
  if(mState == MODE_MOVE)	 //正常运动模式
	{
		if(lastState != mState) //首次检测到正常模式
		{
			lastState = mState;
		}
		LedLightOut(d1);
	}
	else if(mState == MODE_ERR_COM)  //系统通信中断
	{
		if(lastState != mState) //首次检测到通信中断
		{
			 lastState = mState;
			 cnt = 0;
			 ln = 10;  			//亮10s
			 perio = 30;		//周期30s
		}
		else
		{
			cnt ++;
			if(cnt < ln)
			{
				LedLightOut(20); //亮
			}
			else
			{
				LedLightOut(0); //灭
			}
			if(cnt > perio)  cnt = 0;
		}
	}
	else if(mState == MODE_ALARM_VOL)  //系统电池电量过低
	{
		if(lastState != mState) //首次检测到低压
		{
			 lastState = mState;
			 cnt = 0;
			 ln = 5;  			//亮10s
			 perio = 10;		//周期30s
		}
		else
		{
			cnt ++;
			if(cnt < ln)
			{
				LedLightOut(20); //亮
			}
			else
			{
				LedLightOut(0); //灭
			}
			if(cnt > perio)  cnt = 0;
		}
	}
	
}


//控制照明亮度  0~100
void LedLightOut(s16 light)
{
	if(light < 0) light = 0;
	else if(light > 100) light = 100;
	light *= 10;
	
	//改变定时器占空比
	TIM_SetCompare1(TIM3, light);
	TIM_SetCompare2(TIM3, light);
	
}