/**
  ******************************************************************************
  * @file    timer.c
  * @author  zhao linshan
  * @version V0.0.1
  * @date    2016-08-12
  * @brief   ���ļ���Ҫ�Ƕ�Ӳ����ʱ����Դ���г�ʼ������
	*			Ӳ����Դ��	
	*						TIM1  �߼���ʱ��   PWM���ģʽ   ����ˢ������п���			
  *
	*			���ȼ�: (1��)
	*						TIM1  0/3   �����ж�ģʽ��
	
	*			������
	*						void tim_init(void)     	TIM1��ʱ��pwm������ã������

*/

#include "tim.h"

/****************************************************************************
*			TIM1 PWM���ģʽ������ˢ������п��ƣ�
*
*			��168MHz��TIM1ʱ��ԴΪ168MHz��
*					��Ƶ�� �������� 168  
*					����ֵ �������� 20000  20ms
*     ռ�ձȱ仯 �������� 1000 ~ 2000  1.0ms ~ 2.0ms
*     ռ�ձȳ�ֵ 1500
*			���ͨ����
*					PE14  CH1
*					PE13	CH2
*					PE11	CH3
*					PE9   CH4
******************************************************************************/
void tim_init(void)
{	 
		GPIO_InitTypeDef GPIO_InitStructure;	
		NVIC_InitTypeDef  NVIC_InitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;
		
		//��ʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);  
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
		//GPIO����
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9 | GPIO_Pin_11 |GPIO_Pin_13 |GPIO_Pin_14;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOE, &GPIO_InitStructure);

		//�ض���IO��
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);

		//�ж����ȼ�����
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);		

		//����ʱ����Ԫ����
		TIM_DeInit(TIM1);  
		TIM_TimeBaseStructure.TIM_Period = 20000;
		TIM_TimeBaseStructure.TIM_Prescaler = 168 - 1;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

		TIM_OCStructInit(&TIM_OCInitStructure);  
		//ͨ��1
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = 1500;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OC1Init(TIM1, &TIM_OCInitStructure);
		TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
		//ͨ��2 	
		TIM_OCInitStructure.TIM_Pulse = 1522;
		TIM_OC2Init(TIM1, &TIM_OCInitStructure);
		TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
		//ͨ��3
		TIM_OCInitStructure.TIM_Pulse = 1522;
		TIM_OC3Init(TIM1, &TIM_OCInitStructure);
		TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
		//ͨ��4
		TIM_OCInitStructure.TIM_Pulse = 1522;
		TIM_OC4Init(TIM1, &TIM_OCInitStructure);
		TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
		
		//ʹ��ʱ��
		TIM_Cmd(TIM1, ENABLE);		
		TIM_CtrlPWMOutputs(TIM1, ENABLE);  //ʹ��TIM1��PWM���
}




