#include "delay.h"

////////////////////////////////////////////////////////////////////////////////// 	 

#include "includes.h"					//ucos ʹ��	  
#include "os_cfg_app.h"

void delay_init(void)
{
	if(SysTick_Config(SystemCoreClock/OS_CFG_TICK_RATE_HZ))
	{
		while(1);
	}
}

 
//systick�жϷ�����,ʹ��OSʱ�õ�
void SysTick_Handler(void)
{	

		OSIntEnter();								//�����ж�
		OSTimeTick();       				//����ucos��ʱ�ӷ������               
		OSIntExit();       	 				//���������л����ж�
}

void delay_ms(u32 nms)
{
	OS_ERR err;
	OSTimeDlyHMSM(0,0,0,nms,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ200ms
}

			 



































