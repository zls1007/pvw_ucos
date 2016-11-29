#ifndef _USART_H
#define _USART_H
#include "stm32f4xx.h"
#include "stdio.h"	

void uart_init(u32 bound);
int fputc(int ch, FILE *f);

#endif
