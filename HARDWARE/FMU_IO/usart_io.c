#include "usart_io.h"
#include "includes.h"	

extern OS_TCB UsartIOTaskTCB;   		//�����ۺϹ����������
extern OS_MEM MyIOPartition;				//������Ϣ�Ĵ洢��
extern OS_MUTEX MY_USART_IO_MUTEX;  //�Դ��ڲ��������ź���

static CPU_INT08U* MyIOBlkPtr;					//��Ϣ����ָ��
static CPU_INT08U io_buf[20];							//���ڽ������ݻ���
static CPU_INT08U io_cnt = 0;							//���ڽ������ݼ���


//��ʼ��IO��ͨ�Žӿ� ����3
//bound:������115200
void usart_io_init(u32 bound)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
  	USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef  NVIC_InitStructure;
  
		//��ʱ��
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  	
		//IO��ӳ��
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);  
  	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);
	
		//GPIO����
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);  

		//�ж����ȼ�����
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);		
  	
		//����3����
		USART_InitStructure.USART_BaudRate = bound;//����������
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  	USART_InitStructure.USART_StopBits = USART_StopBits_1;
  	USART_InitStructure.USART_Parity = USART_Parity_No;
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
  	USART_Init(USART3, &USART_InitStructure);
		USART_ITConfig(USART3, USART_IT_RXNE,ENABLE);   //��������ж�
  	USART_Cmd(USART3, ENABLE);
}


//�û���Ϣ����
void io_print_hex(USART_TypeDef* USARTx, u8 buf[], u8 size)
{
	OS_ERR err;
	u8 i;
	OSMutexPend(&MY_USART_IO_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err); 	//���󻥳��ź���
	for(i=0; i<size; i++)
	{		
		USART_SendData(USARTx, buf[i]);
  	while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
	}
	OSMutexPost(&MY_USART_IO_MUTEX,OS_OPT_POST_NONE,&err);						//���ͻ����ź���
}

//����3�жϷ������
void USART3_IRQHandler(void)                	
{ 
	u8 c;
	OS_ERR err;
	OSIntEnter();  
  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		c = USART_ReceiveData(USART3);

		//���յ���ʼ֡����ȡһ���ڴ��
		if(io_cnt == 0 && c == 0xa5)
		{

			io_buf[io_cnt ++] = c;
		}
		//���յ���ʼ֡2�������ж�
		 else if(io_cnt == 1 && c != 0x5a)
		 {
				//��������
				io_cnt = 0;			 
		 }
		 else if(io_cnt > 0)
		{

			io_buf[io_cnt ++] = c;
		}
		//���ݽ�����ɣ���������Ϣ
		if(io_cnt > 16)
		{
			io_buf[io_cnt ++] = c;
			if(c == 0xff)  //���ݽ��ճɹ�
			{
				//��ȡ�洢��
				MyIOBlkPtr = (CPU_INT08U* )OSMemGet(&MyIOPartition, &err);
				if(err == OS_ERR_NONE)
				{
					//��������	
					memcpy(MyIOBlkPtr, io_buf, io_cnt);
					//������Ϣ����
					OSTaskQPost((OS_TCB* )&UsartIOTaskTCB,
						(void *)MyIOBlkPtr,
						(OS_MSG_SIZE)io_cnt,
						(OS_OPT)OS_OPT_POST_FIFO,
						(OS_ERR* )&err);
				}
			}			
			MyIOBlkPtr = NULL;
			io_cnt = 0;					
		}
		
	}

	OSIntExit();  											 

} 


 




