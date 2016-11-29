#include "usart_cmd.h"
#include "includes.h"	

extern OS_TCB UsartDealTaskTCB;
extern OS_MEM MyCmdPartition;
extern OS_MUTEX MY_USART_MUTEX; 

static CPU_INT08U* MyDataBlkPtr;			//��Ϣ����ָ��
static CPU_INT08U RxDataCtr = 0;     	//���ռ���
static CPU_INT08U Rx_buf[100];				//�ݴ���յ���ָ��


//��ʼ��IO ����2 
//bound:������
void usart_cmd_init(u32 bound)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
  	USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef  NVIC_InitStructure;
  
		//��ʱ��
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  	
		//IO��ӳ��
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);  
  	GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);
	
		//GPIO����
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);  

		//�ж����ȼ�����
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);		
  	
		//����2����
		USART_InitStructure.USART_BaudRate = bound;//����������
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  	USART_InitStructure.USART_StopBits = USART_StopBits_1;
  	USART_InitStructure.USART_Parity = USART_Parity_No;
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
  	USART_Init(USART2, &USART_InitStructure);
		USART_ITConfig(USART2, USART_IT_RXNE,ENABLE);   //��������ж�
  	USART_Cmd(USART2, ENABLE);
}


//�û���Ϣ����
void os_print_str(USART_TypeDef* USARTx, char *str)
{
	OS_ERR err;
	OSMutexPend(&MY_USART_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err); 	//���󻥳��ź���
	while(*str != '\0')
	{		
		USART_SendData(USARTx, (u8)(*str));
		str ++;
  	while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
	}
	OSMutexPost(&MY_USART_MUTEX,OS_OPT_POST_NONE,&err);						//���ͻ����ź���
}


//����2�жϷ������
void USART2_IRQHandler(void)                	
{ 
	u8 c;
	OS_ERR err;
	OSIntEnter();  
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		c = USART_ReceiveData(USART2);

		//���յ���ʼ֡����ȡһ���ڴ��
		if(c == '{' && RxDataCtr == 0)
		{
			Rx_buf[RxDataCtr ++] = c;
		}
		//���յ�����֡����������Ϣ
		else if(c == '}')
		{
			Rx_buf[RxDataCtr ++] = c;
			
			//��ȡһ���ڴ��
			MyDataBlkPtr = (CPU_INT08U* )OSMemGet(&MyCmdPartition, &err);
			if(err == OS_ERR_NONE)  //��ȡ�ɹ�������������Ϣ����
			{
				//��������	
				memcpy(MyDataBlkPtr, Rx_buf, RxDataCtr);
				//������Ϣ����
				OSTaskQPost((OS_TCB* )&UsartDealTaskTCB,
						(void *)MyDataBlkPtr,
						(OS_MSG_SIZE)RxDataCtr,
						(OS_OPT)OS_OPT_POST_FIFO,
						(OS_ERR* )&err);
			}

			MyDataBlkPtr = NULL;
			RxDataCtr = 0;					
		}
		//���յ����ַ�����������Ϊ��Ч����֡
		else if(RxDataCtr > 80)
		{
			//��������
			RxDataCtr = 0;
		}
		else if(RxDataCtr > 0)
		{
			Rx_buf[RxDataCtr ++] = c;		
		}
		
	}

	OSIntExit();  											 

} 


 




