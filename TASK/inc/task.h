#ifndef __TASK_H
#define __TASK_H

#include "includes.h"

//任务优先级
#define START_TASK_PRIO					3							//开始任务
#define LED_TASK_PRIO						4							//LED任务
#define USART_DEAL_TASK_PRIO		5							//通信控制函数
#define USART_IO_TASK_PRIO			6							//综合板通信控制
#define IMU_DATA_TASK_PRIO			7							//IMU数据采集任务
#define PRESSURE_DATA_TASK_PRIO	11							//气压计采集任务
#define	MOVE_CONTROL_TASK_PRIO	9							//推进器控制任务
#define ADC_DEAL_TASK_PRIO			10						//adc采集数据处理任务
#define BATTERY_DATA_TASK_PRIO  12						//电池数据采集程序

//任务堆栈大小	
#define START_STK_SIZE 					512						//开始任务
#define LED_STK_SIZE 						128						//LED任务
#define USART_DEAL_STK_SIZE     512						//通信控制函数
#define USART_IO_STK_SIZE       256						//综合板通信控制
#define IMU_DATA_STK_SIZE				512						//IMU数据采集任务
#define PRESSURE_DATA_STK_SIZE	256						//气压计采集任务
#define MOVE_CONTROL_STK_SIZE		256						//推进器控制任务
#define ADC_DEAL_STK_SIZE				512						//adc采集数据处理任务
#define BATTERY_DATA_STK_SIZE   256						//电池数据采集程序

//任务函数
void start_task(void *p_arg);									//开始任务
void led_task(void *p_arg);										//LED任务
void usartDeal_task(void *p_arg);							//通信控制函数
void usartIO_task(void *p_arg);								//综合板通信控制
void imuData_task(void *p_arg);								//IMU数据采集任务
void pressureData_task(void *p_arg);					//气压计采集任务
void moveControl_task(void *p_arg);						//推进器控制任务
void adcDeal_task(void *p_arg);								//adc采集数据处理任务
void batteryData_task(void *p_arg);						//电池数据采集程序


//创建任务函数
void start_task_create(void);									//开始任务
void led_task_create(void);										//LED任务
void usartDeal_task_create(void);							//通信控制函数
void usartIO_task_create(void);								//综合板通信控制
void imuData_task_create(void);								//IMU数据采集任务
void pressureData_task_create(void);					//气压计采集任务
void moveControl_task_create(void);						//推进器控制任务
void adcDeal_task_create(void);							  //adc采集数据处理任务
void batteryData_task_create(void);						//电池数据采集程序

#endif
