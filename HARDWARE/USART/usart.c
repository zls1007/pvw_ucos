#include "usart.h"
#include "includes.h"	


//��ʼ��IO ����7
//bound:������
void uart_init(u32 bound)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
  	USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef  NVIC_InitStructure;
  
		//��ʱ��
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART7, ENABLE);
  	
		//IO��ӳ��
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource7, GPIO_AF_UART7);  
  	GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_UART7);
	
		//GPIO����
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_Init(GPIOE, &GPIO_InitStructure);  

		//�ж����ȼ�����
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		NVIC_InitStructure.NVIC_IRQChannel = UART7_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 8;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);		
  	
		//����7����
		USART_InitStructure.USART_BaudRate = 115200;//����������
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  	USART_InitStructure.USART_StopBits = USART_StopBits_1;
  	USART_InitStructure.USART_Parity = USART_Parity_No;
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
  	USART_Init(UART7, &USART_InitStructure);
		USART_ITConfig(UART7, USART_IT_RXNE,ENABLE);   //��������ж�
  	USART_Cmd(UART7, ENABLE);
}

int fputc(int ch, FILE *f)
{
  	USART_SendData(UART7, (u8) ch);
  	while (USART_GetFlagStatus(UART7, USART_FLAG_TC) == RESET);
  	return ch;
}



//����3�жϷ������
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


 




