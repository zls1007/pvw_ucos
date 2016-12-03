/**
  ******************************************************************************
  * @file    timer.c
  * @author  zhao linshan
  * @version V0.0.1
  * @date    2016-08-12
  * @brief   本文件主要是对硬件定时器资源进行初始化配置
	*			硬件资源：	
	*						TIM1  高级定时器   PWM输出模式   对无刷电调进行控制			
	*						TIM3	通用定时器	 PWM输出模式	 对照明进行控制
	*						TIM4	通用定时器	 用作计数器，系统启动前延时
  *
	*			优先级: (1组)
	*						TIM1  0/3   （非中断模式）
	*						TIM3  0/4    (非中断模式)
	
	*			函数：
	*						void tim1_init(void)     	TIM1定时器pwm输出设置（电调）

*/

#include "tim.h"
#include "includes.h"	
extern __IO uint16_t tim4_cnt;

/****************************************************************************
*			TIM1 PWM输出模式（对无刷电调进行控制）
*
*			对168MHz（TIM1时钟源为168MHz）
*					分频数 ———— 168  
*					计数值 ———— 20000  20ms
*     占空比变化 ———— 1000 ~ 2000  1.0ms ~ 2.0ms
*     占空比初值 1500
*			输出通道：
*					PE14  CH1
*					PE13	CH2
*					PE11	CH3
*			低电平通道：
*					PE9   GND  
*					PD13	GND
*					PD14	GND
******************************************************************************/
void tim1_init(void)
{	 
		GPIO_InitTypeDef GPIO_InitStructure;	
		NVIC_InitTypeDef  NVIC_InitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;
		
		//打开时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);  
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
		//GPIO设置(tim通道，复用模式)
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 |GPIO_Pin_13 |GPIO_Pin_14;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOE, &GPIO_InitStructure);
	
		//GPIO 推挽输出PE9
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;					
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;					
		GPIO_Init(GPIOE,&GPIO_InitStructure);
		//GPIO 推挽输出PD13 PD14
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;		
		GPIO_Init(GPIOD,&GPIO_InitStructure);
		//设置为GND
		GPIO_ResetBits(GPIOE,GPIO_Pin_9);
		GPIO_ResetBits(GPIOD,GPIO_Pin_13);
		GPIO_ResetBits(GPIOD,GPIO_Pin_14);

		//重定向IO口
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);

		//中断优先级设置
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);		

		//基本时基单元设置
		TIM_DeInit(TIM1);  
		TIM_TimeBaseStructure.TIM_Period = 20000;
		TIM_TimeBaseStructure.TIM_Prescaler = 168 - 1;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

		TIM_OCStructInit(&TIM_OCInitStructure);  

		//通道2
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		
		TIM_OCInitStructure.TIM_Pulse = 1522;
		TIM_OC2Init(TIM1, &TIM_OCInitStructure);
		TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
		//通道3
		TIM_OCInitStructure.TIM_Pulse = 1522;
		TIM_OC3Init(TIM1, &TIM_OCInitStructure);
		TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
		//通道4
		TIM_OCInitStructure.TIM_Pulse = 1522;
		TIM_OC4Init(TIM1, &TIM_OCInitStructure);
		TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
		
		//使能时钟
		TIM_Cmd(TIM1, ENABLE);		
		TIM_CtrlPWMOutputs(TIM1, ENABLE);  //使能TIM1的PWM输出
}

/****************************************************************************
*			TIM3 PWM输出模式(对照明进行控制)
*
*			对84MHz（TIM3时钟源为84MHz）
*					分频数 ———— 84  
*					计数值 ———— 1000  1KHz
*     占空比变化 ———— 0 ~ 1000  
*     占空比初值 0
*			输出通道：
*					PC6  CH1
*					PC7	 CH2

******************************************************************************/
void tim3_init(void)
{	 
		GPIO_InitTypeDef GPIO_InitStructure;	
		NVIC_InitTypeDef  NVIC_InitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;
		
		//打开时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
		//GPIO设置
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOC, &GPIO_InitStructure);

		//重定向IO口
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);


		//中断优先级设置
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);		

		//基本时基单元设置
		TIM_DeInit(TIM3);  
		TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
		TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

		TIM_OCStructInit(&TIM_OCInitStructure);  
		//通道1
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = 0;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OC1Init(TIM3, &TIM_OCInitStructure);
		TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
		//通道2 	
		TIM_OCInitStructure.TIM_Pulse = 0;
		TIM_OC2Init(TIM3, &TIM_OCInitStructure);
		TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

		
		//使能时钟
		TIM_Cmd(TIM3, ENABLE);		
		TIM_CtrlPWMOutputs(TIM3, ENABLE);  //使能TIM3的PWM输出
}

/****************************************************************************
*			TIM4 计数器
*
*			对84MHz（TIM4时钟源为84MHz）
*					分频数 ———— 84  
*					计数值 ———— 100  0.1ms
******************************************************************************/
void tim4_init(void)
{	 
		NVIC_InitTypeDef  NVIC_InitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		//打开时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  


		//中断优先级设置
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);		

		//基本时基单元设置
		TIM_DeInit(TIM4);  
		TIM_TimeBaseStructure.TIM_Period = 100 - 1;
		TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
		TIM_ClearFlag(TIM4, TIM_FLAG_Update);
		TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

		
		//使能时钟
		//TIM_Cmd(TIM4, ENABLE);		
		TIM_Cmd(TIM4, DISABLE);

}


//中断函数
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4, TIM_FLAG_Update) != RESET)
	{
		TIM_ClearFlag(TIM4, TIM_FLAG_Update);
		tim4_cnt ++;
	}
}



