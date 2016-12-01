#include "task.h"
#include "led_deal.h"

//������ƿ�
OS_TCB LedTaskTCB;
//�����ջ	
CPU_STK LED_TASK_STK[LED_STK_SIZE];

void led_task_create(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	OS_CRITICAL_ENTER();	//�����ٽ���
	//����LED0����
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
	OS_CRITICAL_EXIT();		//�˳��ٽ���	
}

//led0������
void led_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		LedLightCtr();  //�ı�ƹ�����
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ100ms
	}
}
