#include "task.h"
#include "system_param.h"
#include "cmd_deal.h"
//������ƿ�
OS_TCB UsartDealTaskTCB;
//�����ջ	
CPU_STK USART_DEAL_TASK_STK[USART_DEAL_STK_SIZE];

//��������
OS_MEM MyCmdPartition;
CPU_INT08U MyCmdStorage[8][80];

OS_MUTEX MY_USART_MUTEX;   	//����һ���������ź��������ڷ��ʹ�������

void usartDeal_task_create(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	OS_CRITICAL_ENTER();	//�����ٽ���

	//����һ���������ź���
	OSMutexCreate(
							(OS_MUTEX* ) &MY_USART_MUTEX,
							(CPU_CHAR* )"MY_MUTEX",
							(OS_ERR* )&err);
	//����һ������
	OSMemCreate(
							(OS_MEM* ) &MyCmdPartition,
							(CPU_CHAR* ) "My Cmd Partition",
							(void* ) &MyCmdStorage[0][0],
							(OS_MEM_QTY) 8,
							(OS_MEM_SIZE) 80,
							(OS_ERR* )&err);
	
	//��������
	OSTaskCreate((OS_TCB 	* )&UsartDealTaskTCB,		
				 (CPU_CHAR	* )"usart deal task", 		
                 (OS_TASK_PTR )usartDeal_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )USART_DEAL_TASK_PRIO,     	
                 (CPU_STK  *)&USART_DEAL_TASK_STK[0],	
                 (CPU_STK_SIZE)USART_DEAL_STK_SIZE/10,	
                 (CPU_STK_SIZE)USART_DEAL_STK_SIZE,		
                 (OS_MSG_QTY)10,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT    )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP, 
                 (OS_ERR 	* )&err);
	OS_CRITICAL_EXIT();		//�˳��ٽ���	
}

void usartDeal_task(void *p_arg)
{
	OS_ERR err;

	CPU_INT08U *delta;   	//�洢���ָ��
	OS_MSG_SIZE size;			//�洢��ĳ���
	
	Cmd_TypeDef myCmd;					//�洢�û�ָ��Ľṹ��

//CPU_TS ts;  ts = OSTimeGet(&err);
	p_arg = p_arg;
	
	//usart_cmd_init(115200);			//��ʼ��ͨ�Žӿ�
	
	while(1)
	{
		//�ȴ�������Ϣ
		delta = (CPU_INT08U* )OSTaskQPend(0, OS_OPT_PEND_BLOCKING, &size, 0, &err);
		//OSTaskSemPend(0, OS_OPT_PEND_BLOCKING, 0, &err);    //�ȴ������ź���

		CmdAnalysis(&myCmd, delta, size);
		CmdRouter(&myCmd);
		
		//�Ѵ洢��黹������
		OSMemPut(&MyCmdPartition, (void*)delta, &err);

	}
}



