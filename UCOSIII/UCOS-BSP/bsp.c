
#define   BSP_MODULE
#include  <bsp.h>

//硬件初始化  
void bsp_init(void)
{	
	uart_init(115200);					//初始化串口
	
	//初始化MPU9250  MS5611
	tim4_init();								//初始化延时计数
	delay_ms(1000);
	MPU9250_Config();   				//mpu9250 初始化
	MS5611_Config();						//气压计初始化
	
	
	delay_init();               //初始化系统时钟
	LED_Init();									//初始化LED	
	tim1_init();								//初始化pwm输出
	tim3_init();								//初始化pwm输出
	
	//usart_io_init(115200);			//初始化IO板接口
	usart_cmd_init(115200);			//初始化通信接口
	//printf("device init\r\n");
	ADC3_Config();
	I2C_init();									//电池初始化
	
//	
//	tim4_init();
//	delay_ms(1000);
//	MS5611_Config();						//气压计初始化
//	
//	MPU9250_Config();   //mpu9250 初始化
//	
	
	
}


