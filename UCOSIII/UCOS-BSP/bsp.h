#ifndef  BSP_PRESENT
#define  BSP_PRESENT

//ȫ�ֱ����Ƿ����ⲿ����
#ifdef   BSP_MODULE
#define  BSP_EXT
#else
#define  BSP_EXT  extern
#endif

//�����ļ�
#include  <stdio.h>
#include  <stdarg.h>
#include  <cpu.h>
#include  <cpu_core.h>
#include  <lib_def.h>
#include  <lib_ascii.h>
#include  "stm32f4xx.h"

//Ӳ����Դ
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "tim.h"
#include "usart_io.h"
#include "usart_cmd.h"
#include "ms5611.h"
#include "adc.h"
#include "mpu9250.h"

void bsp_init(void);


#endif                                                                                 

