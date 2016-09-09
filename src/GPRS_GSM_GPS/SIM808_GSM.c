/*
 * SIM808_GSM.c
 *
 *  Created on: 2016. szept. 8.
 *      Author: Bence
 */

#include "GPRS_GSM_GPS/SIM808_GSM.h"
#include "GPRS_GSM_GPS/SIM808_driver.h"

#include "string.h"

uint8_t send_SMS(char* text, char* number)
{
  if(strlen(text) > 160)
  {
    return 0;
  }

  char cmd[30];
  char ctrlZ = 0x1A; // <Ctrl + Z> in ASCII

  sprintf(cmd,"%s=\"%s\"\r","AT+CMGS",number);
  if(check_resp_SIM808(cmd, ">", 1, 500))
  {
    put_s_SIM808(text);
    if(send_n_wait_for_resp(&ctrlZ, 13, 500))
    {
      // TODO: process response
      return 1;
    }

  }

  return 0;
}
