/*
 * SIM808_GSM.c
 *
 *  Created on: 2016. szept. 8.
 *      Author: Bence
 */

#include "GPRS_GSM_GPS/SIM808_GSM.h"
#include "GPRS_GSM_GPS/SIM808_driver.h"

boolean_t send_SMS(char* text, char* number)
{
  if(length(text) > 160)
  {
    return False;
  }

  char cmd[30];
  char ctrlZ = 0x1A; // <Ctrl + Z> in ASCII

  sprintf(cmd,"AT+CMGS=\"%s\"\r", number);
  put_s_SIM808(cmd);

  while('>' != get_c_SIM808());

  put_s_SIM808(text);
  put_c_SIM808(ctrlZ);
  get_s_SIM808(4, NULL);

  return True;
}
