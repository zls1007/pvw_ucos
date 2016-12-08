#include "task.h"
#include "system_param.h"
#include "cmd_deal.h"
#include "adc.h"

//任务控制块
OS_TCB adcDealTaskTCB;
//任务堆栈	
CPU_STK ADC_DEAL_TASK_STK[ADC_DEAL_STK_SIZE];

//创建一个信号量
OS_SEM MyADCSem;

//存储AD转换的结果
uint16_t ADC_ConvertedValue[N][2];

void adcDeal_task_create(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	OS_CRITICAL_ENTER();	//进入临界区
	
	//创建信号量
	OSSemCreate(&MyADCSem,
							"my adc sem",
							0,
							&err);
	
	//创建任务
	OSTaskCreate((OS_TCB 	* )&adcDealTaskTCB,		
				 (CPU_CHAR	* )"adc deal task", 		
                 (OS_TASK_PTR )adcDeal_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )ADC_DEAL_TASK_PRIO,     	
                 (CPU_STK  *)&ADC_DEAL_TASK_STK[0],	
                 (CPU_STK_SIZE)ADC_DEAL_STK_SIZE/10,	
                 (CPU_STK_SIZE)ADC_DEAL_STK_SIZE,		
                 (OS_MSG_QTY)0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT    )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP, 
                 (OS_ERR 	* )&err);
	OS_CRITICAL_EXIT();		//退出临界区	
}

//均值滤波部分
void filter()
{
	float sum = 0;
	float temp;
	float deep, vol;
	float cur = 1.2;
	float ADC_Filter[2];
	uint16_t ADC_Value[N][2];
	SysState_Type mState;
	static float deep_filter[5];  //移动平均滤波
	static u8 index = 0;
	
	u8 count, i;
	
	//复制数据
	memcpy(ADC_Value, ADC_ConvertedValue, N*2*2);
	
	//均值滤波
	for(i=0; i<2; i++)
	{
		for(count=0; count<140; count ++)
		{									   
			temp = ADC_Value[count][i] * 3.3/0x0fff;
			sum += temp;

		}
		ADC_Filter[i] = sum/140;
		sum = 0;
	}

	//printf("adc=%.2f, %.2f\r\n", ADC_Filter[0], ADC_Filter[1]);
	
	deep = ADC_Filter[1] * 1000/165;	//转换成电流 mA
	//deep = (deep - 4)*10;	 			//转换为深度值  16mA = 1.6Mpa   1Mpa = 100m
	deep = (deep - 4)*100/16;	 			//转换为深度值  16mA = 1Mpa   1Mpa = 100m

	vol =  ADC_Filter[0] * 4;			// *4

	
	//移动平均滤波
	deep_filter[index++] = deep;
	if(index == 5) index = 0;
	sum = 0;
	for(i=0; i<5; i++)
	{
		sum += deep_filter[i];	
	}
	deep = sum/5;
	
	deep += 0.6;
	vol += 0.5;

	//更新传感器参数
	SetDeepData(&deep);
	//SetBatteryData(&vol, &cur);
	//printf("vol=%.2f, deep=%.2f\r\n", vol, deep);
	
//	//检测是否欠压
//	if(vol < 10.7)
//	{
//		mState = MODE_ALARM_VOL;
//		SetSysState(&mState);
//	}
		
}


void adcDeal_task(void *p_arg)
{
	OS_ERR err;
	
//CPU_TS ts;  ts = OSTimeGet(&err);
	p_arg = p_arg;
	
	while(1)
	{
		//等待信号量
		//OSSemPend(&MyADCSem, 0, OS_OPT_PEND_BLOCKING, 0, &err);
		OSTaskSemPend(0, OS_OPT_PEND_BLOCKING, 0, &err);
		filter();
		//printf("adc\r\n");

		


	}
}




