/*
 * stm32f0xx_it.h
 *
 *  Created on: 2016. jul. 18.
 *      Author: Bence
 */

#ifndef __STM32F0xx_IT_H
#define __STM32F0xx_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 

void SysTick_Handler(void);
void ADC1_IRQHandler(void);
void USART1_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F0xx_IT_H */

