
#define   BSP_MODULE
#include  <bsp.h>

//Ӳ����ʼ��  
void bsp_init(void)
{	
  delay_init();               //��ʼ��ϵͳʱ��
	LED_Init();									//��ʼ��LED	
	uart_init(115200);					//��ʼ������
	tim_init();									//��ʼ��pwm���
	usart_io_init(115200);			//��ʼ��IO��ӿ�
	usart_cmd_init(115200);			//��ʼ��ͨ�Žӿ�
	MS5611_Config();						//��ѹ�Ƴ�ʼ��
	
}


