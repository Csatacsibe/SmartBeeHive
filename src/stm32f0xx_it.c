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
    ext_LED(2);
  }
}
