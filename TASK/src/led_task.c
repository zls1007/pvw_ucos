#include "task.h"
#include "led_deal.h"

//任务控制块
OS_TCB LedTaskTCB;
//任务堆栈	
CPU_STK LED_TASK_STK[LED_STK_SIZE];

void led_task_create(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	OS_CRITICAL_ENTER();	//进入临界区
	//创建LED0任务
	OSTaskCreate((OS_TCB 	* )&LedTaskTCB,		
				 (CPU_CHAR	* )"led task", 		
                 (OS_TASK_PTR )led_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )LED_TASK_PRIO,     
                 (CPU_STK   * )&LED_TASK_STK[0],	
                 (CPU_STK_SIZE)LED_STK_SIZE/10,	
                 (CPU_STK_SIZE)LED_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP,
                 (OS_ERR 	* )&err);		
	OS_CRITICAL_EXIT();		//退出临界区	
}

//led0任务函数
void led_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		LedLightCtr();  //改变灯光亮度
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err); //延时100ms
	}
}
