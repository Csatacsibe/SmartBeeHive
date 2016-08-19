/*
 * SIM808_driver.c
 *
 *  Created on: 2016. aug. 11.
 *      Author: Bence
 */

#include "SIM808_driver.h"

#include "usart.h"
#include "gpio.h"

#include "constants.h"

#include "string.h"

static boolean_t get_SIM808_status(uint8_t timeout);

static boolean_t get_SIM808_status(uint8_t timeout)
{
  uint8_t resp[7];

  put_s_SIM808((uint8_t*)"AT\r");

  if(HAL_OK == get_s_SIM808(resp, 6, timeout))
  {
    if(strncmp((char*)resp, "\\r\\nOK\\r\\n", 6) == 0)
    {
      return True;
    }
  }

  return False;
}

HAL_StatusTypeDef get_s_SIM808(uint8_t* s, uint8_t size, uint8_t timeout)
{
  return HAL_UART_Receive(&huart1, (uint8_t *)s, size, timeout);
}

void put_c_SIM808(uint8_t c)
{
  HAL_UART_Transmit(&huart1, &c, 1, 0xFFFFFF);
}

void put_s_SIM808(uint8_t* string)
{
  HAL_UART_Transmit(&huart1, string, strlen((char*)string), 0xFFFFFF);
}

void GPS_ant_pwr(boolean_t val)
{
  if(val == True)
  {
    HAL_GPIO_WritePin(GPS_ANT_EN_GPIO_Port, GPS_ANT_EN_Pin, GPIO_PIN_SET);
  }
  else if(val == False)
  {
    HAL_GPIO_WritePin(GPS_ANT_EN_GPIO_Port, GPS_ANT_EN_Pin, GPIO_PIN_RESET);
  }
}

void power_SIM808()
{
  HAL_GPIO_WritePin(SIM_PWR_KEY_GPIO_Port, SIM_PWR_KEY_Pin, GPIO_PIN_RESET);
  HAL_Delay(2000);
  HAL_GPIO_WritePin(SIM_PWR_KEY_GPIO_Port, SIM_PWR_KEY_Pin, GPIO_PIN_SET);
}

void reset_SIM808()
{
  HAL_GPIO_WritePin(SIM_RST_GPIO_Port, SIM_RST_Pin, GPIO_PIN_RESET);
  HAL_Delay(150);
  HAL_GPIO_WritePin(SIM_RST_GPIO_Port, SIM_RST_Pin, GPIO_PIN_SET);
}

void configure_SIM808()
{
  if(True == get_SIM808_status(30))
  {
    put_s_SIM808((uint8_t*)"ATE0\r");   // disable command echo mode
  }
}

void SIM808_init()
{
  GPS_ant_pwr(False);  // default value: GPS antenna pwr off
  configure_SIM808();  // send default config if the module is powered
}




