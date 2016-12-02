#include "adc.h"
#include "includes.h"	

__IO uint16_t ADC_ConvertedValue[N][2];

extern OS_TCB adcDealTaskTCB;				//任务控制块儿

extern OS_MEM MyADCPartition;				//传递消息的存储块
static CPU_INT08U* MyADCBlkPtr;			//存储块指针

#define ADC3_DR_Address ((u32)0x40012200+0x4c)

/**********************************************************************************
ADC3 配置函数   电压、电流采样  84MHz
	PA2  adc_2  电压测量
	PC1	 adc_11 深度测量

***********************************************************************************/
void ADC3_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;		
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//打开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);


	//pa2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//pC1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	DMA_DeInit(DMA2_Stream0);
	DMA_InitStructure.DMA_Channel = DMA_Channel_2;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC3_DR_Address;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC_ConvertedValue;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = 2*N;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);
  DMA_Cmd(DMA2_Stream0, ENABLE);
	
	//打开DMA转换完成中断
	DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);
	


	//中断优先级设置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	  
	 RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);    
   RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE);

	//ADC通用配置
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div6;    //14MHz
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);
	
	//adc初始化

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 2;
  ADC_Init(ADC3, &ADC_InitStructure);

	//ADC温度校准
	ADC_TempSensorVrefintCmd(ENABLE);   


  ADC_RegularChannelConfig(ADC3, ADC_Channel_2, 1, ADC_SampleTime_480Cycles);  //14M/(20+480)=28k
	ADC_RegularChannelConfig(ADC3, ADC_Channel_11, 2, ADC_SampleTime_480Cycles);
	
	
	ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);
	ADC_DMACmd(ADC3, ENABLE);
	
	//ADC3使能
	ADC_Cmd(ADC3, ENABLE);

	//??ADC1??
	ADC_SoftwareStartConv(ADC3);     
}


/**********************************************************************************
DMA转换完成中断服务函数   28k/2/140 = 100

***********************************************************************************/
void DMA2_Stream0_IRQHandler(void)
{
		OS_ERR err;
	
	if(DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0) != RESET)
	{
		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
		printf("ad\r\n");
//		//获取存储块
//		MyADCBlkPtr = (CPU_INT08U* )OSMemGet(&MyADCPartition, &err);
//		if(err == OS_ERR_NONE)
//		{
//			//复制数据	
//			memcpy(MyADCBlkPtr, (const void *)ADC_ConvertedValue, N*2*2);
//			//发送消息队列
//			OSTaskQPost((OS_TCB* )&adcDealTaskTCB,
//				(void *)MyADCBlkPtr,
//				(OS_MSG_SIZE)N*2*2,
//				(OS_OPT)OS_OPT_POST_FIFO,
//				(OS_ERR* )&err);
//		}
	}
}

