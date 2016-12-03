#include "task.h"
#include "ms5611.h"
#include "param.h"

//������ƿ�
OS_TCB	PressureDataTaskTCB;
//�����ջ
CPU_STK	PRESSURE_DATA_TASK_STK[PRESSURE_DATA_STK_SIZE];


void pressureData_task_create(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	OS_CRITICAL_ENTER();	//�����ٽ���
	
	//��������
	OSTaskCreate((OS_TCB 	* )&PressureDataTaskTCB,		
				 (CPU_CHAR	* )"pressure data task", 		
                 (OS_TASK_PTR )pressureData_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )PRESSURE_DATA_TASK_PRIO,     	
                 (CPU_STK  *)&PRESSURE_DATA_TASK_STK[0],	
                 (CPU_STK_SIZE)PRESSURE_DATA_STK_SIZE/10,	
                 (CPU_STK_SIZE)PRESSURE_DATA_STK_SIZE,		
                 (OS_MSG_QTY)0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT    )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP, 
                 (OS_ERR 	* )&err);
	OS_CRITICAL_EXIT();		//�˳��ٽ���	
}

void pressureData_task(void *p_arg)
{
	OS_ERR err;
	float pressure,temper; //ǻ����ѹ��оƬ�¶�
	
	
	//MS5611_Config();						//��ѹ�Ƴ�ʼ��

	while(1)
	{
		//��ȡ�¶Ⱥ�ѹ��ֵ
		temper = MS5611_getTemperature(CMD_CONVERT_D2_OSR4096);
    pressure= MS5611_getPressure(CMD_CONVERT_D1_OSR4096);
		printf("pre=%.3f  tem=%.3f\r\n", pressure, temper);
		
		//����ϵͳ����
		SetPTData(&pressure, &temper);
		
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ500ms
	}
}



