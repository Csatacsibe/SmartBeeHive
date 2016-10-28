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
void RTC_IRQHandler(void);
void EXTI0_1_IRQHandler(void);
void EXTI4_15_IRQHandler(void);
void DMA1_Channel1_IRQHandler(void);
void ADC1_IRQHandler(void);
void USART1_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F0xx_IT_H */

