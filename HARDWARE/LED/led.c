#include "led.h"

//初始化PB1为输出.并使能时钟	    
//LED IO初始化
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//打开时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	//GPIO配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_11;			//选定引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;					//设置为普通输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//输出频率为100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;					//设置为上拉   GPIO_PuPd_NOPULL(不上拉)	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	//关闭小灯
	GPIO_SetBits(GPIOB,GPIO_Pin_1);
	GPIO_SetBits(GPIOB,GPIO_Pin_3);
	GPIO_SetBits(GPIOB,GPIO_Pin_11);
}

