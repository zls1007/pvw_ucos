#include "task.h"
#include "system_param.h"
#include "cmd_deal.h"
//任务控制块
OS_TCB UsartDealTaskTCB;
//任务堆栈	
CPU_STK USART_DEAL_TASK_STK[USART_DEAL_STK_SIZE];

//创建分区
OS_MEM MyCmdPartition;
CPU_INT08U MyCmdStorage[8][80];

OS_MUTEX MY_USART_MUTEX;   	//定义一个互斥型信号量，用于访问公共串口

void usartDeal_task_create(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	OS_CRITICAL_ENTER();	//进入临界区

	//创建一个互斥型信号量
	OSMutexCreate(
							(OS_MUTEX* ) &MY_USART_MUTEX,
							(CPU_CHAR* )"MY_MUTEX",
							(OS_ERR* )&err);
	//创建一个分区
	OSMemCreate(
							(OS_MEM* ) &MyCmdPartition,
							(CPU_CHAR* ) "My Cmd Partition",
							(void* ) &MyCmdStorage[0][0],
							(OS_MEM_QTY) 8,
							(OS_MEM_SIZE) 80,
							(OS_ERR* )&err);
	
	//创建任务
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
	OS_CRITICAL_EXIT();		//退出临界区	
}

void usartDeal_task(void *p_arg)
{
	OS_ERR err;

	CPU_INT08U *delta;   	//存储块的指针
	OS_MSG_SIZE size;			//存储块的长度
	
	Cmd_TypeDef myCmd;					//存储用户指令的结构体

//CPU_TS ts;  ts = OSTimeGet(&err);
	p_arg = p_arg;
	
	//usart_cmd_init(115200);			//初始化通信接口
	
	while(1)
	{
		//等待任务消息
		delta = (CPU_INT08U* )OSTaskQPend(0, OS_OPT_PEND_BLOCKING, &size, 0, &err);
		//OSTaskSemPend(0, OS_OPT_PEND_BLOCKING, 0, &err);    //等待任务信号量

		CmdAnalysis(&myCmd, delta, size);
		CmdRouter(&myCmd);
		
		//把存储块归还到分区
		OSMemPut(&MyCmdPartition, (void*)delta, &err);

	}
}




