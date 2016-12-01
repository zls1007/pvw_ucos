#include "task.h"
#include "moter_control.h"
#include "system_param.h"

//������ƿ�
OS_TCB	MoveControlTaskTCB;
//�����ջ
CPU_STK	MOVE_CONTROL_TASK_STK[MOVE_CONTROL_STK_SIZE];


void moveControl_task_create(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	OS_CRITICAL_ENTER();	//�����ٽ���
	
	//��������
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
	OS_CRITICAL_EXIT();		//�˳��ٽ���	
}

//������
void moveControl_task(void *p_arg)
{
	OS_ERR err;
	CPU_TS ts;  //ts = OSTimeGet(&err);
	MoveMode_Type  myMoveType;  //�˶�ģʽ
	SysState_Type  mySysState;  //ϵͳ״̬
	static MoveMode_Type myLastState = MODE_MANUAL;  //��¼��һ��ѭ��ʱ�Ĺ���ģʽ
	while(1)
	{
		GetSysState(&mySysState);
		if(mySysState != MODE_MOVE)    //���ϵͳ��������������״̬
		{
			MoterMoveStop();  //ֹͣ�˶�
			continue;
		}
		
		GetMoveMode(&myMoveType);
		if(myMoveType == MODE_MANUAL)  	//�ֶ�ģʽ
		{
			manualControlFuc(myLastState);
			myLastState = MODE_MANUAL;
		}
		else if(myMoveType == MODE_PITCH) //������̬����
		{
			pitchControlFuc(myLastState);
			myLastState = MODE_PITCH;
		}
		else if(myMoveType == MODE_DEEP) //��ȿ���
		{
			deepControlFuc(myLastState);
			myLastState = MODE_DEEP;
		}
		else if(myMoveType == MODE_DIR)	//�������ģʽ
		{
			dirControlFuc(myLastState);
			myLastState = MODE_DIR;
		}
		else if(myMoveType == MODE_BOTH) //ȫ�ջ�ģʽ
		{
			allControlFuc(myLastState);
			myLastState = MODE_BOTH;
		}
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ10ms
	}
}




