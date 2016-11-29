#include "delay.h"

////////////////////////////////////////////////////////////////////////////////// 	 

#include "includes.h"					//ucos 使用	  
#include "os_cfg_app.h"

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

void delay_ms(u32 nms)
{
	OS_ERR err;
	OSTimeDlyHMSM(0,0,0,nms,OS_OPT_TIME_HMSM_STRICT,&err); //延时200ms
}

			 



































