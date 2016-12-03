/*
 * SIM808_GSM.c
 *
 *  Created on: 2016. szept. 8.
 *      Author: Bence
 */

#include "GPRS_GSM_GPS/SIM808_GSM.h"
#include "GPRS_GSM_GPS/SIM808_driver.h"
#include <GPRS_GSM_GPS/SIM808_device.h>

#include "device_management.h"

#include "string.h"

static void check_SMS_sending_result(char* data);
static boolean_t SMS_succes = False;

boolean_t send_SMS(char* text, char* number)
{
  if(length(text) > 160)
  {
    return False;
  }

  char cmd[30] = {0};
  char ctrlZ = 0x1A; // <Ctrl + Z> in ASCII
  SMS_succes = False;

  sprintf(cmd,"AT+CMGS=\"%s\"\r", number);
  put_s_SIM808(cmd);

  while('>' != get_c_SIM808()); // TODO: timeout

  put_s_SIM808(text);
  put_c_SIM808(ctrlZ);
  get_s_SIM808(4, check_SMS_sending_result);

  return waitFor(&SMS_succes, 5000);
}

static void check_SMS_sending_result(char* data)
{
  if(check_msg_header_SIM808(data, "+CMGS") == False)
  {
    SMS_succes = False;
  }
  else
  {
    SMS_succes = True;
  }
}
