#ifndef __TASK_H
#define __TASK_H

#include "includes.h"

//�������ȼ�
#define START_TASK_PRIO					3							//��ʼ����
#define LED_TASK_PRIO						4							//LED����
#define USART_DEAL_TASK_PRIO		5							//ͨ�ſ��ƺ���
#define USART_IO_TASK_PRIO			6							//�ۺϰ�ͨ�ſ���
#define IMU_DATA_TASK_PRIO			7							//IMU���ݲɼ�����
#define PRESSURE_DATA_TASK_PRIO	11							//��ѹ�Ʋɼ�����
#define	MOVE_CONTROL_TASK_PRIO	9							//�ƽ�����������
#define ADC_DEAL_TASK_PRIO			10						//adc�ɼ����ݴ�������
#define BATTERY_DATA_TASK_PRIO  12						//������ݲɼ�����

//�����ջ��С	
#define START_STK_SIZE 					512						//��ʼ����
#define LED_STK_SIZE 						128						//LED����
#define USART_DEAL_STK_SIZE     512						//ͨ�ſ��ƺ���
#define USART_IO_STK_SIZE       256						//�ۺϰ�ͨ�ſ���
#define IMU_DATA_STK_SIZE				512						//IMU���ݲɼ�����
#define PRESSURE_DATA_STK_SIZE	256						//��ѹ�Ʋɼ�����
#define MOVE_CONTROL_STK_SIZE		256						//�ƽ�����������
#define ADC_DEAL_STK_SIZE				512						//adc�ɼ����ݴ�������
#define BATTERY_DATA_STK_SIZE   256						//������ݲɼ�����

//������
void start_task(void *p_arg);									//��ʼ����
void led_task(void *p_arg);										//LED����
void usartDeal_task(void *p_arg);							//ͨ�ſ��ƺ���
void usartIO_task(void *p_arg);								//�ۺϰ�ͨ�ſ���
void imuData_task(void *p_arg);								//IMU���ݲɼ�����
void pressureData_task(void *p_arg);					//��ѹ�Ʋɼ�����
void moveControl_task(void *p_arg);						//�ƽ�����������
void adcDeal_task(void *p_arg);								//adc�ɼ����ݴ�������
void batteryData_task(void *p_arg);						//������ݲɼ�����


//����������
void start_task_create(void);									//��ʼ����
void led_task_create(void);										//LED����
void usartDeal_task_create(void);							//ͨ�ſ��ƺ���
void usartIO_task_create(void);								//�ۺϰ�ͨ�ſ���
void imuData_task_create(void);								//IMU���ݲɼ�����
void pressureData_task_create(void);					//��ѹ�Ʋɼ�����
void moveControl_task_create(void);						//�ƽ�����������
void adcDeal_task_create(void);							  //adc�ɼ����ݴ�������
void batteryData_task_create(void);						//������ݲɼ�����

#endif
