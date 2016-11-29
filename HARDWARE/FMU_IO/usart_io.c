#include "usart_io.h"
#include "includes.h"	

extern OS_TCB UsartIOTaskTCB;   		//处理综合管理板的任务块
extern OS_MEM MyIOPartition;				//传递消息的存储块
extern OS_MUTEX MY_USART_IO_MUTEX;  //对串口操作互斥信号量

static CPU_INT08U* MyIOBlkPtr;					//消息队列指针
static CPU_INT08U io_buf[20];							//串口接收数据缓存
static CPU_INT08U io_cnt = 0;							//串口接收数据计数


//初始化IO板通信接口 串口3
//bound:波特率115200
void usart_io_init(u32 bound)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
  	USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef  NVIC_InitStructure;
  
		//打开时钟
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  	
		//IO重映射
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);  
  	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);
	
		//GPIO配置
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);  

		//中断优先级设置
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);		
  	
		//串口3配置
		USART_InitStructure.USART_BaudRate = bound;//波特率设置
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  	USART_InitStructure.USART_StopBits = USART_StopBits_1;
  	USART_InitStructure.USART_Parity = USART_Parity_No;
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
  	USART_Init(USART3, &USART_InitStructure);
		USART_ITConfig(USART3, USART_IT_RXNE,ENABLE);   //接收完成中断
  	USART_Cmd(USART3, ENABLE);
}


//用户消息发送
void io_print_hex(USART_TypeDef* USARTx, u8 buf[], u8 size)
{
	OS_ERR err;
	u8 i;
	OSMutexPend(&MY_USART_IO_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err); 	//请求互斥信号量
	for(i=0; i<size; i++)
	{		
		USART_SendData(USARTx, buf[i]);
  	while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
	}
	OSMutexPost(&MY_USART_IO_MUTEX,OS_OPT_POST_NONE,&err);						//发送互斥信号量
}

//串口3中断服务程序
void USART3_IRQHandler(void)                	
{ 
	u8 c;
	OS_ERR err;
	OSIntEnter();  
  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		c = USART_ReceiveData(USART3);

		//接收到开始帧，获取一个内存块
		if(io_cnt == 0 && c == 0xa5)
		{

			io_buf[io_cnt ++] = c;
		}
		//接收到开始帧2，进行判断
		 else if(io_cnt == 1 && c != 0x5a)
		 {
				//计数清零
				io_cnt = 0;			 
		 }
		 else if(io_cnt > 0)
		{

			io_buf[io_cnt ++] = c;
		}
		//数据接收完成，给任务发消息
		if(io_cnt > 16)
		{
			io_buf[io_cnt ++] = c;
			if(c == 0xff)  //数据接收成功
			{
				//获取存储块
				MyIOBlkPtr = (CPU_INT08U* )OSMemGet(&MyIOPartition, &err);
				if(err == OS_ERR_NONE)
				{
					//复制数据	
					memcpy(MyIOBlkPtr, io_buf, io_cnt);
					//发送消息队列
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


 




