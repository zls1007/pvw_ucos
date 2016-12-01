#include "led_deal.h"
//0.1s

//�����ƿ���
void LedLightCtr(void)
{
	SysState_Type mState;   			//ϵͳ����״̬
	s16 d1, d2;										//�û�����ƹ�����
	
	static u8 cnt = 0;						//��������ֵ
	static u16 ln, perio;  				//�������ںͳ���
	static SysState_Type lastState = MODE_MOVE; //��¼��һ�ε�ϵͳ����״̬
	
	//��ȡ��ֵ
	GetSysState(&mState);
	GetLedLight(&d1, &d2);
	
  if(mState == MODE_MOVE)	 //�����˶�ģʽ
	{
		if(lastState != mState) //�״μ�⵽����ģʽ
		{
			lastState = mState;
		}
		LedLightOut(d1);
	}
	else if(mState == MODE_ERR_COM)  //ϵͳͨ���ж�
	{
		if(lastState != mState) //�״μ�⵽ͨ���ж�
		{
			 lastState = mState;
			 cnt = 0;
			 ln = 10;  			//��10s
			 perio = 30;		//����30s
		}
		else
		{
			cnt ++;
			if(cnt < ln)
			{
				LedLightOut(20); //��
			}
			else
			{
				LedLightOut(0); //��
			}
			if(cnt > perio)  cnt = 0;
		}
	}
	else if(mState == MODE_ALARM_VOL)  //ϵͳ��ص�������
	{
		if(lastState != mState) //�״μ�⵽��ѹ
		{
			 lastState = mState;
			 cnt = 0;
			 ln = 5;  			//��10s
			 perio = 10;		//����30s
		}
		else
		{
			cnt ++;
			if(cnt < ln)
			{
				LedLightOut(20); //��
			}
			else
			{
				LedLightOut(0); //��
			}
			if(cnt > perio)  cnt = 0;
		}
	}
	
}


//������������  0~100
void LedLightOut(s16 light)
{
	if(light < 0) light = 0;
	else if(light > 100) light = 100;
	light *= 10;
	
	//�ı䶨ʱ��ռ�ձ�
	TIM_SetCompare1(TIM3, light);
	TIM_SetCompare2(TIM3, light);
	
}