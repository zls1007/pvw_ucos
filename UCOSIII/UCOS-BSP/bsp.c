
#define   BSP_MODULE
#include  <bsp.h>

//Ӳ����ʼ��  
void bsp_init(void)
{	
	uart_init(115200);					//��ʼ������
	
	//��ʼ��MPU9250  MS5611
	tim4_init();								//��ʼ����ʱ����
	delay_ms(1000);
	MPU9250_Config();   				//mpu9250 ��ʼ��
	MS5611_Config();						//��ѹ�Ƴ�ʼ��
	
	
	delay_init();               //��ʼ��ϵͳʱ��
	LED_Init();									//��ʼ��LED	
	tim1_init();								//��ʼ��pwm���
	tim3_init();								//��ʼ��pwm���
	
	//usart_io_init(115200);			//��ʼ��IO��ӿ�
	usart_cmd_init(115200);			//��ʼ��ͨ�Žӿ�
	//printf("device init\r\n");
	ADC3_Config();
	I2C_init();									//��س�ʼ��
	
//	
//	tim4_init();
//	delay_ms(1000);
//	MS5611_Config();						//��ѹ�Ƴ�ʼ��
//	
//	MPU9250_Config();   //mpu9250 ��ʼ��
//	
	
	
}


