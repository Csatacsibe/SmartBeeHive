/*
 * SIM808_driver.c
 *
 *  Created on: 2016. aug. 11.
 *      Author: Bence
 */

#include "SIM808_driver.h"

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

void SIM808_init()
{
  GPS_ant_pwr(False); // default value: GPS antenna pwr off
}

void switch_SIM808()
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
