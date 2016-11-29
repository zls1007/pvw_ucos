#ifndef _USART_CMD_H
#define _USART_CMD_H
#include "stm32f4xx.h"

void usart_cmd_init(u32 bound);

void os_print_str(USART_TypeDef* USARTx, char *str);


#endif
