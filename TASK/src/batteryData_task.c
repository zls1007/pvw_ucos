#include "task.h"
#include "param.h"
//任务控制块
OS_TCB	BatteryDataTaskTCB;
//任务堆栈
CPU_STK	BATTERY_DATA_TASK_STK[BATTERY_DATA_STK_SIZE];



void batteryData_task_create(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	OS_CRITICAL_ENTER();	//进入临界区
	
	//创建任务
	OSTaskCreate((OS_TCB 	* )&BatteryDataTaskTCB,		
				 (CPU_CHAR	* )"Battery data task", 		
                 (OS_TASK_PTR )batteryData_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )BATTERY_DATA_TASK_PRIO,     	
                 (CPU_STK  *)&BATTERY_DATA_TASK_STK[0],	
                 (CPU_STK_SIZE)BATTERY_DATA_STK_SIZE/10,	
                 (CPU_STK_SIZE)BATTERY_DATA_STK_SIZE,		
                 (OS_MSG_QTY)0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT    )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP, 
                 (OS_ERR 	* )&err);
	OS_CRITICAL_EXIT();		//退出临界区	
}



void batteryData_task(void *p_arg)
{
	OS_ERR err;
	float temper, vol, cur;
	s8 relcap, abscap;
	SysState_Type mState;
	
	float vol_t, cur_t;

	CPU_SR_ALLOC();


	while(1)
	{
		ReadBatteryInfo(&temper, &vol, &cur, &relcap, &abscap);
		GetBatteryData(&vol_t, &cur_t);
		if(vol != -1) vol_t = vol;
		if(cur != -1) cur_t = cur;
		SetBatteryData(&vol_t, &cur_t);
		//printf("temper=%.2f, vol=%.2f, cur=%.2f, relcap=%d, abscap=%d\r\n", temper, vol, cur, relcap, abscap);
		
		//检测是否欠压
		if(vol_t < 15.6)
		{
			mState = MODE_ALARM_VOL;
			SetSysState(&mState);
		}
		//printf("vol=%.2f, cur=%.2f\r\n", vol_t, cur_t);
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err); //延时200ms
	}
}



