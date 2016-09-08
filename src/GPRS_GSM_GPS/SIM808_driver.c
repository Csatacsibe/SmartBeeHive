/*
 * SIM808_driver.c
 *
 *  Created on: 2016. aug. 11.
 *      Author: Bence
 */

#include <STM32_bsp/constants.h>
#include <STM32_bsp/gpio.h>
#include <STM32_bsp/usart.h>
#include <device_management.h>
#include <GPRS_GSM_GPS/SIM808_driver.h>

#include "string.h"

uint8_t rx_buffer_SIM808[160];
boolean_t rx_cmplt = False;

static boolean_t get_SIM808_status(uint32_t to);

static boolean_t get_SIM808_status(uint32_t to)
{
  return check_resp_SIM808("AT\r","OK", 7, to);
}

boolean_t check_resp_SIM808(char* msg, char* pattern, uint8_t length, uint8_t to)
{
  put_s_SIM808(msg);
  HAL_UART_Receive_IT(&huart1, rx_buffer_SIM808, length);

  if(waitFor(&rx_cmplt, to) == True)
  {
    if(strstr((char*)rx_buffer_SIM808, pattern) != NULL)
    {
      return True;
    }
  }

  return False;
}

void put_c_SIM808(char c)
{
  HAL_UART_Transmit(&huart1, &c, 1, 0xFFFFFF);
}

void put_s_SIM808(char* string)
{
  HAL_UART_Transmit(&huart1, (uint8_t*)string, strlen((char*)string), 0xFFFFFF);
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
  HAL_Delay(1100);
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
  if(True == get_SIM808_status(200))
  {
    put_s_SIM808((uint8_t*)"ATE0\r");   // disable command echo mode
  }
}

void SIM808_init()
{
  HAL_GPIO_WritePin(SIM_PWR_KEY_GPIO_Port, SIM_PWR_KEY_Pin, GPIO_PIN_SET); // Sets GPIO to High-Z, pulled up in the SIM808 internally
  HAL_GPIO_WritePin(SIM_RST_GPIO_Port, SIM_RST_Pin, GPIO_PIN_SET);         // Sets GPIO to High-Z, externally pulled up

  GPS_ant_pwr(False);  // default value: GPS antenna power off
  configure_SIM808();  // send default configuration if the module is powered
}




