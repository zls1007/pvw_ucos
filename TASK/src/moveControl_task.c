#include "task.h"
#include "moter_control.h"
#include "system_param.h"

//任务控制块
OS_TCB	MoveControlTaskTCB;
//任务堆栈
CPU_STK	MOVE_CONTROL_TASK_STK[MOVE_CONTROL_STK_SIZE];


void moveControl_task_create(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	OS_CRITICAL_ENTER();	//进入临界区
	
	//创建任务
	OSTaskCreate((OS_TCB 	* )&MoveControlTaskTCB,		
				 (CPU_CHAR	* )"move control task", 		
                 (OS_TASK_PTR )moveControl_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )MOVE_CONTROL_TASK_PRIO,     	
                 (CPU_STK  *)&MOVE_CONTROL_TASK_STK[0],	
                 (CPU_STK_SIZE)MOVE_CONTROL_STK_SIZE/10,	
                 (CPU_STK_SIZE)MOVE_CONTROL_STK_SIZE,		
                 (OS_MSG_QTY)0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT    )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP, 
                 (OS_ERR 	* )&err);
	OS_CRITICAL_EXIT();		//退出临界区	
}

//任务函数
void moveControl_task(void *p_arg)
{
	OS_ERR err;
	CPU_TS ts;  //ts = OSTimeGet(&err);
	MoveMode_Type  myMoveType;  //运动模式
	SysState_Type  mySysState;  //系统状态
	static MoveMode_Type myLastState = MODE_MANUAL;  //记录上一次循环时的工作模式
	while(1)
	{
		GetSysState(&mySysState);
		if(mySysState != MODE_MOVE)    //如果系统不处于正常工作状态
		{
			MoterMoveStop();  //停止运动
			continue;
		}
		
		GetMoveMode(&myMoveType);
		if(myMoveType == MODE_MANUAL)  	//手动模式
		{
			manualControlFuc(myLastState);
			myLastState = MODE_MANUAL;
		}
		else if(myMoveType == MODE_PITCH) //俯仰姿态控制
		{
			pitchControlFuc(myLastState);
			myLastState = MODE_PITCH;
		}
		else if(myMoveType == MODE_DEEP) //深度控制
		{
			deepControlFuc(myLastState);
			myLastState = MODE_DEEP;
		}
		else if(myMoveType == MODE_DIR)	//方向控制模式
		{
			dirControlFuc(myLastState);
			myLastState = MODE_DIR;
		}
		else if(myMoveType == MODE_BOTH) //全闭环模式
		{
			allControlFuc(myLastState);
			myLastState = MODE_BOTH;
		}
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err); //延时10ms
	}
}




