
#include "start_task.h"
#include "system_param.h"
#include "config_param.h"
#include "bsp.h"


int main(void)
{
  OS_ERR err;
	
  
	//硬件初始化
	bsp_init();
	
	//系统全局参数初始化
	config_param_init();   //配置参数初始化
	system_param_init();   //系统参数初始化
	
	//初始化UCOSIII
	OSInit(&err);	

	//创建开始任务
	start_task_create();

	//开启UCOSIII
	OSStart(&err);      
  
	while(1);
}








