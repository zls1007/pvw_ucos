#include "task.h"
#include "system_param.h"
#include "io_deal.h"
//任务控制块
OS_TCB UsartIOTaskTCB;
//任务堆栈	
CPU_STK USART_IO_TASK_STK[USART_IO_STK_SIZE];

//创建分区
OS_MEM MyIOPartition;
CPU_INT08U MyIOStorage[5][40];

OS_MUTEX MY_USART_IO_MUTEX;   	//定义一个互斥型信号量，用于访问公共串口

void usartIO_task_create(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	OS_CRITICAL_ENTER();	//进入临界区

	//创建一个互斥型信号量
	OSMutexCreate(
							(OS_MUTEX* ) &MY_USART_IO_MUTEX,
							(CPU_CHAR* )"MY_MUTEX",
							(OS_ERR* )&err);
	//创建一个分区
	OSMemCreate(
							(OS_MEM* ) &MyIOPartition,
							(CPU_CHAR* ) "My IO Partition",
							(void* ) &MyIOStorage[0][0],
							(OS_MEM_QTY) 5,
							(OS_MEM_SIZE) 40,
							(OS_ERR* )&err);
	
	//创建任务
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
	OS_CRITICAL_EXIT();		//退出临界区	
}

//定义任务函数
void usartIO_task(void *p_arg)
{
	OS_ERR err;

	CPU_INT08U *delta;   	//存储块的指针
	OS_MSG_SIZE size;			//存储块的长度


//CPU_TS ts;  ts = OSTimeGet(&err);
	p_arg = p_arg;
	
	//usart_io_init(115200);			//初始化IO板接口
	
	while(1)
	{
		//等待任务消息
		delta = (CPU_INT08U* )OSTaskQPend(0, OS_OPT_PEND_BLOCKING, &size, 0, &err);

		//对接受的到的消息进行处理
		IOMsgAnalysis(delta, size);
		//printf("size=%d\r\n",size);
		//把存储块归还到分区
		OSMemPut(&MyIOPartition, (void*)delta, &err);

	}
}




