/*
 * stm32f0xx_it.c
 *
 *  Created on: 2016. jul. 18.
 *      Author: Bence
 */

#include <GPRS_GSM_GPS/SIM808_device.h>
#include <GPRS_GSM_GPS/SIM808_driver.h>
#include <device_management.h>
#include <weight_measurement.h>
#include <STM32_bsp/adc.h>

#include <state_machine.h>

#include "string.h"

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
  //HAL_UART_IRQHandler(&huart1);

  /* UART parity error interrupt occurred -------------------------------------*/
  if((__HAL_UART_GET_IT(&huart1, UART_IT_PE) != RESET) && (__HAL_UART_GET_IT_SOURCE(&huart1, UART_IT_PE) != RESET))
  {
    __HAL_UART_CLEAR_IT(&huart1, UART_CLEAR_PEF);

    huart1.ErrorCode |= HAL_UART_ERROR_PE;
    /* Set the UART state ready to be able to start again the process */
    huart1.State = HAL_UART_STATE_READY;
  }

  /* UART frame error interrupt occurred --------------------------------------*/
  if((__HAL_UART_GET_IT(&huart1, UART_IT_FE) != RESET) && (__HAL_UART_GET_IT_SOURCE(&huart1, UART_IT_ERR) != RESET))
  {
    __HAL_UART_CLEAR_IT(&huart1, UART_CLEAR_FEF);

    huart1.ErrorCode |= HAL_UART_ERROR_FE;
    /* Set the UART state ready to be able to start again the process */
    huart1.State = HAL_UART_STATE_READY;
  }

  /* UART noise error interrupt occurred --------------------------------------*/
  if((__HAL_UART_GET_IT(&huart1, UART_IT_NE) != RESET) && (__HAL_UART_GET_IT_SOURCE(&huart1, UART_IT_ERR) != RESET))
  {
    __HAL_UART_CLEAR_IT(&huart1, UART_CLEAR_NEF);

    huart1.ErrorCode |= HAL_UART_ERROR_NE;
    /* Set the UART state ready to be able to start again the process */
    huart1.State = HAL_UART_STATE_READY;
  }

  /* UART Over-Run interrupt occurred -----------------------------------------*/
  if((__HAL_UART_GET_IT(&huart1, UART_IT_ORE) != RESET) && (__HAL_UART_GET_IT_SOURCE(&huart1, UART_IT_ERR) != RESET))
  {
    __HAL_UART_CLEAR_IT(&huart1, UART_CLEAR_OREF);

    huart1.ErrorCode |= HAL_UART_ERROR_ORE;
    /* Set the UART state ready to be able to start again the process */
    huart1.State = HAL_UART_STATE_READY;
  }

  /* UART in mode Receiver ---------------------------------------------------*/
  if((__HAL_UART_GET_IT(&huart1, UART_IT_RXNE) != RESET) && (__HAL_UART_GET_IT_SOURCE(&huart1, UART_IT_RXNE) != RESET))
  {
    uint16_t uhMask = huart1.Mask;

    if((huart1.State == HAL_UART_STATE_BUSY_RX) || (huart1.State == HAL_UART_STATE_BUSY_TX_RX))
    {
      //*huart1.pRxBuffPtr++ = (uint8_t)(huart1.Instance->RDR & (uint8_t)uhMask);
      uint8_t byte = (uint8_t)(huart1.Instance->RDR & (uint8_t)uhMask);
      if(byte == '\r')
      {
        cr_cnt++;
      }

      if(rx_cnt < 159)
      {
        rx_buffer_SIM808[rx_cnt++] = byte;
      }
      else
      {
         // TODO: buffer overflow handling
      }

      if(cr_cnt == cr_limit)
      {
        rx_buffer_SIM808[rx_cnt] = 0;
        rx_cnt = 0;
        cr_cnt = 0;

        __HAL_UART_DISABLE_IT(&huart1, UART_IT_RXNE);

        // Check if a transmit Process is ongoing or not
        if(huart1.State == HAL_UART_STATE_BUSY_TX_RX)
        {
          huart1.State = HAL_UART_STATE_BUSY_TX;
        }
        else
        {
          // Disable the UART Parity Error Interrupt
          __HAL_UART_DISABLE_IT(&huart1, UART_IT_PE);

          // Disable the UART Error Interrupt: (Frame error, noise error, overrun error)
          __HAL_UART_DISABLE_IT(&huart1, UART_IT_ERR);

          huart1.State = HAL_UART_STATE_READY;
        }

        if(rx_callback_SIM808 != NULL)
        {
          char* data = (char*)rx_buffer_SIM808;

          if(strstr(data, "OK") != NULL)
          {
            rx_callback_SIM808(data);
          }
          else
          {
            rx_error = True;
          }
        }
      }
    }

    // Clear RXNE interrupt flag
    __HAL_UART_SEND_REQ(&huart1, UART_RXDATA_FLUSH_REQUEST);
  }
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

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
  if(hrtc->Instance == RTC)
  {
    measure_RTC_1min();

    inc_wake_up_counter();
  }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if( GPIO_Pin == CHG_STAT_Pin)
  {
    // TODO: handle charger
  }

  if( GPIO_Pin == GY_INT1_Pin)
  {
    if(state_SBH != ALARM_THEFT)
    {
      //state_SBH = ALARM_RAISED;
    }
  }

  if( GPIO_Pin == GY_INT2_Pin)
  {

  }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle)
{
  if(AdcHandle->Instance == ADC1)
  {
    HAL_ADC_Stop_DMA(AdcHandle);
    if(scale_callback != NULL)
    {
      scale_callback();
    }
  }
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* AdcHandle)
{
  if(AdcHandle->Instance == ADC1)
  {

  }
}
