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

//��ֵ�˲�����
void filter(uint16_t ADC_ConvertedValue[][2])
{
	float sum = 0;
	float temp;
	float deep, vol;
	float cur = 1.2;
	float ADC_Filter[2];
	SysState_Type mState;
	static float deep_filter[5];  //�ƶ�ƽ���˲�
	static u8 index = 0;
	
	u8 count, i;
	for(i=0; i<2; i++)
	{
		for(count=0; count<140; count ++)
		{									   
			temp = ADC_ConvertedValue[count][i] * 3.3/0x0fff;
			sum += temp;
		}
		ADC_Filter[i] = sum/140;
		sum = 0;
	}

	deep = ADC_Filter[1] * 1000/165;	//????? mA
	deep = (deep - 4)*10;	 			//?????  16mA = 1.6Mpa   1Mpa = 100m
	deep = (deep)*100/16;	 			//?????  16mA = 1Mpa   1Mpa = 100m

	vol =  ADC_Filter[0] * 4;			// ??4


	//�ƶ�ƽ���˲�
	deep_filter[index++] = deep;
	if(index == 5) index = 0;
	sum = 0;
	for(i=0; i<5; i++)
	{
		sum += deep_filter[i];	
	}
	deep = sum/5;

	//���´���������
	SetDeepData(&deep);
	GetBatteryData(&vol, &cur);
	printf("vol=%.2f, deep=%.2f\r\n", vol, deep);
	
	//����Ƿ�Ƿѹ
	if(vol < 10.7)
	{
		mState = MODE_ALARM_VOL;
		SetSysState(&mState);
	}
		
}


void adcDeal_task(void *p_arg)
{
	OS_ERR err;

	uint16_t *delta;   	//�洢���ָ��
	OS_MSG_SIZE size;			//�洢��ĳ���
	
//CPU_TS ts;  ts = OSTimeGet(&err);
	p_arg = p_arg;
	
	
	while(1)
	{
		//�ȴ�������Ϣ
		delta = (uint16_t* )OSTaskQPend(0, OS_OPT_PEND_BLOCKING, &size, 0, &err);
		
		filter((uint16_t(*)[2])delta);

		
		//�Ѵ洢��黹������
		OSMemPut(&MyADCPartition, (void*)delta, &err);

	}
}




