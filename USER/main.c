
#include "start_task.h"
#include "system_param.h"


int main(void)
{
  OS_ERR err;
  
	//Ӳ����ʼ��
	bsp_init();
	
	//ϵͳȫ�ֲ�����ʼ��
	//mySys_init();
	
	//��ʼ��UCOSIII
	OSInit(&err);	

	//������ʼ����
	start_task_create();

	//����UCOSIII
	OSStart(&err);      
  
	while(1);
}








