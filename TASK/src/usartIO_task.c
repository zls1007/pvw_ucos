#include "task.h"
#include "system_param.h"
#include "io_deal.h"
//������ƿ�
OS_TCB UsartIOTaskTCB;
//�����ջ	
CPU_STK USART_IO_TASK_STK[USART_IO_STK_SIZE];

//��������
OS_MEM MyIOPartition;
CPU_INT08U MyIOStorage[5][40];

OS_MUTEX MY_USART_IO_MUTEX;   	//����һ���������ź��������ڷ��ʹ�������

void usartIO_task_create(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	OS_CRITICAL_ENTER();	//�����ٽ���

	//����һ���������ź���
	OSMutexCreate(
							(OS_MUTEX* ) &MY_USART_IO_MUTEX,
							(CPU_CHAR* )"MY_MUTEX",
							(OS_ERR* )&err);
	//����һ������
	OSMemCreate(
							(OS_MEM* ) &MyIOPartition,
							(CPU_CHAR* ) "My IO Partition",
							(void* ) &MyIOStorage[0][0],
							(OS_MEM_QTY) 5,
							(OS_MEM_SIZE) 40,
							(OS_ERR* )&err);
	
	//��������
	OSTaskCreate((OS_TCB 	* )&UsartIOTaskTCB,		
				 (CPU_CHAR	* )"usart IO task", 		
                 (OS_TASK_PTR )usartIO_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )USART_IO_TASK_PRIO,     	
                 (CPU_STK  *)&USART_IO_TASK_STK[0],	
                 (CPU_STK_SIZE)USART_IO_STK_SIZE/10,	
                 (CPU_STK_SIZE)USART_IO_STK_SIZE,		
                 (OS_MSG_QTY)5,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT    )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP, 
                 (OS_ERR 	* )&err);
	OS_CRITICAL_EXIT();		//�˳��ٽ���	
}

//����������
void usartIO_task(void *p_arg)
{
	OS_ERR err;

	CPU_INT08U *delta;   	//�洢���ָ��
	OS_MSG_SIZE size;			//�洢��ĳ���


//CPU_TS ts;  ts = OSTimeGet(&err);
	p_arg = p_arg;
	
	//usart_io_init(115200);			//��ʼ��IO��ӿ�
	
	while(1)
	{
		//�ȴ�������Ϣ
		delta = (CPU_INT08U* )OSTaskQPend(0, OS_OPT_PEND_BLOCKING, &size, 0, &err);

		//�Խ��ܵĵ�����Ϣ���д���
		IOMsgAnalysis(delta, size);
		//printf("size=%d\r\n",size);
		//�Ѵ洢��黹������
		OSMemPut(&MyIOPartition, (void*)delta, &err);

	}
}




