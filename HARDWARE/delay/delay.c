#include "delay.h"

////////////////////////////////////////////////////////////////////////////////// 	 

#include "includes.h"					//ucos ʹ��	  
#include "os_cfg_app.h"

__IO uint16_t tim4_cnt;

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

void os_delay_ms(u32 nms)
{
	OS_ERR err;
	OSTimeDlyHMSM(0,0,0,nms,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ200ms
}

void delay_ms(u32 nms)
{
	tim4_cnt = 0;
	TIM_Cmd(TIM4, ENABLE);	
	nms *= 10;
	while(tim4_cnt < nms);
	TIM_Cmd(TIM4, DISABLE);
}


			 



































