/*
 * SIM808_device.c
 *
 *  Created on: 2016. szept. 8.
 *      Author: Bence
 */

#include "GPRS_GSM_GPS/SIM808_device.h"
#include "GPRS_GSM_GPS/SIM808_driver.h"

void set_netlight(uint8_t value)
{
  char cmd[20];
  sprintf(cmd, "AT+CNETLIGHT=%d\r", value);
  put_s_SIM808(cmd);
}

uint8_t get_netlight()
{
  if(send_n_wait_for_resp("AT+CNETLIGHT?\r", 17, 200) == True)
  {
    // TODO: process response
  }

  return 255;
}
