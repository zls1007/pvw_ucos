#ifndef _DELAY_H
#define _DELAY_H
#include "stm32f4xx.h" 


void delay_init(void);
void delay_ms(u32 nms);
void os_delay_ms(u32 nms);

#endif

