/*
 * usart.h
 *
 *  Created on: 2016. jul. 18.
 *      Author: Bence
 */

#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f0xx_hal.h"

extern UART_HandleTypeDef huart1;

void MX_USART1_UART_Init(void);

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */

