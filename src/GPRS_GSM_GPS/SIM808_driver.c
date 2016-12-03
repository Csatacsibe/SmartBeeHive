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

uint8_t rx_buffer_SIM808[200];
boolean_t rx_cmplt = False, rx_error = False;
uint8_t rx_cnt = 0, cr_cnt = 0, cr_limit = 2;
SIM808_rx_callback_t rx_callback_SIM808 = NULL;

static boolean_t powered = False;

uint16_t length(char* message)
{
  uint16_t i;

  for(i = 0; message[i] != 0; i++)
  {

  }

  return i;
}

boolean_t is_powered_SIM808(void)
{
  return powered;
}

void put_c_SIM808(uint8_t c)
{
  HAL_UART_Transmit(&huart1, &c, 1, 1000);
}

uint8_t get_c_SIM808()
{
  uint8_t retval = 0;
  HAL_UART_Receive(&huart1, &retval, 1, 5000);
  return retval;
}

uint16_t put_s_SIM808(char* string)
{
  uint16_t size = length(string);
  HAL_UART_Transmit(&huart1, (uint8_t*)string, size, 1000);
  return size;
}

void get_s_SIM808(uint8_t cr_lmt, SIM808_rx_callback_t callback)
{
  cr_limit = cr_lmt;
  rx_callback_SIM808 = callback;
  rx_error = False;

  HAL_UART_Receive_IT(&huart1, rx_buffer_SIM808, 1);
}

void enable_GPS_antenna(boolean_t val)
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

  powered = ((powered == False) ? True : False);
}

void reset_SIM808()
{
  HAL_GPIO_WritePin(SIM_RST_GPIO_Port, SIM_RST_Pin, GPIO_PIN_RESET);
  HAL_Delay(150);
  HAL_GPIO_WritePin(SIM_RST_GPIO_Port, SIM_RST_Pin, GPIO_PIN_SET);
}

void SIM808_init()
{
  HAL_GPIO_WritePin(SIM_PWR_KEY_GPIO_Port, SIM_PWR_KEY_Pin, GPIO_PIN_SET); // Sets GPIO to High-Z, pulled up in the SIM808 internally
  HAL_GPIO_WritePin(SIM_RST_GPIO_Port, SIM_RST_Pin, GPIO_PIN_SET);         // Sets GPIO to High-Z, externally pulled up

  enable_GPS_antenna(False);  // default value: GPS antenna power off
  //configure_SIM808();  // send default configuration if the module is powered
}




