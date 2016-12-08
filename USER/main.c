
#include "start_task.h"
#include "system_param.h"
#include "config_param.h"
#include "bsp.h"


int main(void)
{
  OS_ERR err;
	
  
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








