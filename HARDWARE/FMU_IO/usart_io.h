#ifndef _USART_IO_H
#define _USART_IO_H
#include "stm32f4xx.h"
#include "stdio.h"	

void usart_io_init(u32 bound);
void io_print_hex(USART_TypeDef* USARTx, u8 *buf, u8 size);


#endif
