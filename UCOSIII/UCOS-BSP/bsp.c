
#define   BSP_MODULE
#include  <bsp.h>

//硬件初始化  
void bsp_init(void)
{	
  delay_init();               //初始化系统时钟
	LED_Init();									//初始化LED	
	uart_init(115200);					//初始化串口
	tim1_init();									//初始化pwm输出
	tim3_init();									//初始化pwm输出
	//usart_io_init(115200);			//初始化IO板接口
	usart_cmd_init(115200);			//初始化通信接口
	//MS5611_Config();						//气压计初始化
	ADC3_Config();
	//MPU9250_Config();   //mpu9250 初始化
	
	
}


