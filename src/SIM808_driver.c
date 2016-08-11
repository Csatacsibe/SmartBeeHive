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
