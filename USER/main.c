
#include "start_task.h"
#include "system_param.h"


int main(void)
{
  OS_ERR err;
  
	//硬件初始化
	bsp_init();
	
	//系统全局参数初始化
	//mySys_init();
	
	//初始化UCOSIII
	OSInit(&err);	

	//创建开始任务
	start_task_create();

	//开启UCOSIII
	OSStart(&err);      
  
	while(1);
}








