
#include "start_task.h"
#include "system_param.h"
#include "config_param.h"
#include "bsp.h"


int main(void)
{
  OS_ERR err;
	
	uart_init(115200);					//��ʼ������
	
	//��ʼ��MPU9250  MS5611
	tim4_init();
	delay_ms(1000);
	MPU9250_Config();   //mpu9250 ��ʼ��
	MS5611_Config();						//��ѹ�Ƴ�ʼ��
  
	//Ӳ����ʼ��
	bsp_init();
	
	//ϵͳȫ�ֲ�����ʼ��
	config_param_init();   //���ò�����ʼ��
	system_param_init();   //ϵͳ������ʼ��
	
	//��ʼ��UCOSIII
	OSInit(&err);	

	//������ʼ����
	start_task_create();

	//����UCOSIII
	OSStart(&err);      
  
	while(1);
}








