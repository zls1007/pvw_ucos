
#define   BSP_MODULE
#include  <bsp.h>

//Ӳ����ʼ��  
void bsp_init(void)
{	
	
	delay_init();               //��ʼ��ϵͳʱ��
	LED_Init();									//��ʼ��LED	
	//uart_init(115200);					//��ʼ������
	tim1_init();									//��ʼ��pwm���
	tim3_init();									//��ʼ��pwm���
	
	//usart_io_init(115200);			//��ʼ��IO��ӿ�
	usart_cmd_init(115200);			//��ʼ��ͨ�Žӿ�
	//printf("device init\r\n");
	ADC3_Config();
//	
//	tim4_init();
//	delay_ms(1000);
//	MS5611_Config();						//��ѹ�Ƴ�ʼ��
//	
//	MPU9250_Config();   //mpu9250 ��ʼ��
//	
	
	
}


