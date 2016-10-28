/*
 * stm32f0xx_it.c
 *
 *  Created on: 2016. jul. 18.
 *      Author: Bence
 */

#include <GPRS_GSM_GPS/SIM808_driver.h>
#include <device_management.h>
#include "stm32f0xx_hal.h"
#include "stm32f0xx.h"
#include "stm32f0xx_it.h"

extern ADC_HandleTypeDef hadc;
extern UART_HandleTypeDef huart1;
extern RTC_HandleTypeDef hrtc;
extern DMA_HandleTypeDef hdma_adc;

/******************************************************************************/
/*            Cortex-M0 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

void SysTick_Handler(void)
{
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
}

void ADC1_IRQHandler()
{
  HAL_ADC_IRQHandler(&hadc);
}

void USART1_IRQHandler()
{
  HAL_UART_IRQHandler(&huart1);
}

void RTC_IRQHandler(void)
{
  HAL_RTC_AlarmIRQHandler(&hrtc);
}

void EXTI0_1_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(CHG_STAT_Pin);
}

void EXTI4_15_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GY_INT1_Pin);
  HAL_GPIO_EXTI_IRQHandler(GY_INT2_Pin);
}

void DMA1_Channel1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_adc);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    rx_cmplt = True;

    if(check_in_isr == True)
    {
      check_in_isr = False;
      //TODO: call processing functions here
    }
  }
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
  if(hrtc->Instance == RTC)
  {
    measure_RTC_1min();
  }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if( GPIO_Pin == CHG_STAT_Pin)
  {

  }

  if( GPIO_Pin == GY_INT1_Pin)
  {

  }

  if( GPIO_Pin == GY_INT2_Pin)
  {

  }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle)
{

}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* AdcHandle)
{

}
