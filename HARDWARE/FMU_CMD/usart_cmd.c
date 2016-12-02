#include "usart_cmd.h"
#include "includes.h"	

extern OS_TCB UsartDealTaskTCB;
extern OS_MEM MyCmdPartition;
extern OS_MUTEX MY_USART_MUTEX; 

static CPU_INT08U* MyDataBlkPtr;			//消息队列指针
static CPU_INT08U RxDataCtr = 0;     	//接收计数
static CPU_INT08U Rx_buf[100];				//暂存接收到的指令


//初始化IO 串口1
//bound:波特率
void usart_cmd_init(u32 bound)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
  	USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef  NVIC_InitStructure;
  
		//打开时钟
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  	
		//IO重映射
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART2);  
  	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART2);
	
		//GPIO配置
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);  

		//中断优先级设置
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);		
  	
		//串口1配置
		USART_InitStructure.USART_BaudRate = bound;//波特率设置
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  	USART_InitStructure.USART_StopBits = USART_StopBits_1;
  	USART_InitStructure.USART_Parity = USART_Parity_No;
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
  	USART_Init(USART1, &USART_InitStructure);
		USART_ITConfig(USART1, USART_IT_RXNE,ENABLE);   //接收完成中断
  	USART_Cmd(USART1, ENABLE);
}


//用户消息发送
void os_print_str(USART_TypeDef* USARTx, char *str)
{
	OS_ERR err;
	OSMutexPend(&MY_USART_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err); 	//请求互斥信号量
	while(*str != '\0')
	{		
		USART_SendData(USARTx, (u8)(*str));
		str ++;
  	while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
	}
	OSMutexPost(&MY_USART_MUTEX,OS_OPT_POST_NONE,&err);						//发送互斥信号量
}


//串口1中断服务程序
void USART1_IRQHandler(void)                	
{ 
	u8 c;
	OS_ERR err;
	OSIntEnter();  
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		c = USART_ReceiveData(USART1);

		//接收到开始帧，获取一个内存块
		if(c == '{' && RxDataCtr == 0)
		{
			Rx_buf[RxDataCtr ++] = c;
		}
		//接收到结束帧，给任务发消息
		else if(c == '}')
		{
			Rx_buf[RxDataCtr ++] = c;
			
			//获取一个内存块
			MyDataBlkPtr = (CPU_INT08U* )OSMemGet(&MyCmdPartition, &err);
			if(err == OS_ERR_NONE)  //获取成功，给任务发送消息队列
			{
				//复制数据	
				memcpy(MyDataBlkPtr, Rx_buf, RxDataCtr);
				//发送消息队列
				OSTaskQPost((OS_TCB* )&UsartDealTaskTCB,
						(void *)MyDataBlkPtr,
						(OS_MSG_SIZE)RxDataCtr,
						(OS_OPT)OS_OPT_POST_FIFO,
						(OS_ERR* )&err);
			}

			MyDataBlkPtr = NULL;
			RxDataCtr = 0;					
		}
		//接收到的字符串超长，设为无效数据帧
		else if(RxDataCtr > 80)
		{
			//计数清零
			RxDataCtr = 0;
		}
		else if(RxDataCtr > 0)
		{
			Rx_buf[RxDataCtr ++] = c;		
		}
		
	}

	OSIntExit();  											 

} 


 




