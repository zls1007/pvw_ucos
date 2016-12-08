#include "delay.h"

////////////////////////////////////////////////////////////////////////////////// 	 

#include "includes.h"					//ucos 使用	  
#include "os_cfg_app.h"

__IO uint16_t tim4_cnt;

void delay_init(void)
{
	if(SysTick_Config(SystemCoreClock/OS_CFG_TICK_RATE_HZ))
	{
		while(1);
	}
}

 
//systick中断服务函数,使用OS时用到
void SysTick_Handler(void)
{	

		OSIntEnter();								//进入中断
		OSTimeTick();       				//调用ucos的时钟服务程序               
		OSIntExit();       	 				//触发任务切换软中断
}

void os_delay_ms(u32 nms)
{
	OS_ERR err;
	OSTimeDlyHMSM(0,0,0,nms,OS_OPT_TIME_HMSM_STRICT,&err); //延时200ms
}

void delay_ms(u32 nms)
{
	tim4_cnt = 0;
	TIM_Cmd(TIM4, ENABLE);	
	nms *= 10;
	while(tim4_cnt < nms);
	TIM_Cmd(TIM4, DISABLE);
}


			 



































