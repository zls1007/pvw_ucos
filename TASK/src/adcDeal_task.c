#include "task.h"
#include "system_param.h"
#include "cmd_deal.h"
//������ƿ�
OS_TCB adcDealTaskTCB;
//�����ջ	
CPU_STK ADC_DEAL_TASK_STK[ADC_DEAL_STK_SIZE];

//��������
OS_MEM MyADCPartition;
CPU_INT08U MyADCStorage[5][600];


void adcDeal_task_create(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	OS_CRITICAL_ENTER();	//�����ٽ���

	//����һ������
	OSMemCreate(
							(OS_MEM* ) &MyADCPartition,
							(CPU_CHAR* ) "My ADC Partition",
							(void* ) &MyADCStorage[0][0],
							(OS_MEM_QTY) 5,
							(OS_MEM_SIZE) 600,
							(OS_ERR* )&err);
	
	//��������
	OSTaskCreate((OS_TCB 	* )&adcDealTaskTCB,		
				 (CPU_CHAR	* )"adc deal task", 		
                 (OS_TASK_PTR )adcDeal_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )ADC_DEAL_TASK_PRIO,     	
                 (CPU_STK  *)&ADC_DEAL_TASK_STK[0],	
                 (CPU_STK_SIZE)ADC_DEAL_STK_SIZE/10,	
                 (CPU_STK_SIZE)ADC_DEAL_STK_SIZE,		
                 (OS_MSG_QTY)10,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT    )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP, 
                 (OS_ERR 	* )&err);
	OS_CRITICAL_EXIT();		//�˳��ٽ���	
}

void adcDeal_task(void *p_arg)
{
	OS_ERR err;

	CPU_INT08U *delta;   	//�洢���ָ��
	OS_MSG_SIZE size;			//�洢��ĳ���
	
//CPU_TS ts;  ts = OSTimeGet(&err);
	p_arg = p_arg;
	
	
	while(1)
	{
		//�ȴ�������Ϣ
		delta = (CPU_INT08U* )OSTaskQPend(0, OS_OPT_PEND_BLOCKING, &size, 0, &err);

		
		//�Ѵ洢��黹������
		OSMemPut(&MyADCPartition, (void*)delta, &err);

	}
}




