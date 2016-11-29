#include "usart.h"
#include "includes.h"	


//初始化IO 串口7
//bound:波特率
void uart_init(u32 bound)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
  	USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef  NVIC_InitStructure;
  
		//打开时钟
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART7, ENABLE);
  	
		//IO重映射
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource7, GPIO_AF_UART7);  
  	GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_UART7);
	
		//GPIO配置
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_Init(GPIOE, &GPIO_InitStructure);  

		//中断优先级设置
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		NVIC_InitStructure.NVIC_IRQChannel = UART7_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 8;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);		
  	
		//串口7配置
		USART_InitStructure.USART_BaudRate = 115200;//波特率设置
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  	USART_InitStructure.USART_StopBits = USART_StopBits_1;
  	USART_InitStructure.USART_Parity = USART_Parity_No;
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
  	USART_Init(UART7, &USART_InitStructure);
		USART_ITConfig(UART7, USART_IT_RXNE,ENABLE);   //接收完成中断
  	USART_Cmd(UART7, ENABLE);
}

int fputc(int ch, FILE *f)
{
  	USART_SendData(UART7, (u8) ch);
  	while (USART_GetFlagStatus(UART7, USART_FLAG_TC) == RESET);
  	return ch;
}



//串口3中断服务程序
void UART7_IRQHandler(void)                	
{ 
	u8 c;
	OS_ERR err;
	OSIntEnter();  
  if(USART_GetITStatus(UART7, USART_IT_RXNE) != RESET)
	{
		c = USART_ReceiveData(UART7);
	}

	OSIntExit();  											 

} 


 




