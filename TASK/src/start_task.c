#include "task.h"
#include "led_task.h"
#include "usartDeal_task.h"
#include "imuData_task.h"
#include "usartIO_task.h"
#include "pressureData_task.h"

//任务控制块
OS_TCB StartTaskTCB;
//任务堆栈	
CPU_STK START_TASK_STK[START_STK_SIZE];


void start_task_create(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	OS_CRITICAL_ENTER();//进入临界区
	//创建开始任务
	OSTaskCreate(		(OS_TCB 	* )&StartTaskTCB,			//任务控制块
									(CPU_CHAR	* )"start task", 			//任务名字
                 (OS_TASK_PTR )start_task, 				//任务函数
                 (void		* )0,										//传递给任务函数的参数
                 (OS_PRIO	  )START_TASK_PRIO,     //任务优先级
                 (CPU_STK   * )&START_TASK_STK[0],	//任务堆栈基地址
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//任务堆栈深度限位
                 (CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void   	* )0,					//用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP, //任务选项,为了保险起见，所有任务都保存浮点寄存器的值
                 (OS_ERR 	* )&err);				//存放该函数错误时的返回值
	OS_CRITICAL_EXIT();	//退出临界区	 
}

//开始任务函数
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,设置默认的时间片长度s
	OSSchedRoundRobinCfg(DEF_ENABLED,10,&err);  
#endif		
	
	//创建灯光闪烁任务  4
	led_task_create();
	
	//创建串口处理任务  5
	usartDeal_task_create();
	
	//创建IO板通信任务  6
	//usartIO_task_create();
	
	//创建imu接收任务  7
	imuData_task_create();	
	
	//创建气压计采集任务  8
	pressureData_task_create();
	
	//创建推进器控制程序 9
	moveControl_task_create();
						 
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//挂起开始任务			 
}
