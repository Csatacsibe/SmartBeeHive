/*
 * SIM808_device.c
 *
 *  Created on: 2016. szept. 8.
 *      Author: Bence
 */

#include "GPRS_GSM_GPS/SIM808_device.h"
#include "GPRS_GSM_GPS/SIM808_driver.h"
#include "device_management.h"

#include "string.h"

static float temperature = 0;
static uint16_t vcc = 0;

static boolean_t ok = False;

static void process_CMTE_resp(char* data);
static void process_CBC_resp(char* data);
static void process_CREG_resp(char* data);

boolean_t is_SIM_registered_SIM808()
{
  ok = False;

  put_s_SIM808("AT+CREG?\r");
  get_s_SIM808(4, process_CREG_resp);

  return waitFor(&ok, 1000);
}

static void process_CREG_resp(char* data)
{
  if(check_msg_header_SIM808(data, "+CREG") == False)
  {
    ok = False;
    return;
  }

  while(data[0] != ',')
  {
    data++;
  }
  data++;

  if((data[0] - 48) == 1)
  {
    ok = True;
  }
  else
  {
    ok = False;
  }
}

void set_netlight(uint8_t value)
{
  char cmd[20];
  sprintf(cmd, "AT+CNETLIGHT=%d\r", value);
  put_s_SIM808(cmd);
}

void cmd_tmp_SIM808()
{
  put_s_SIM808("AT+CMTE?\r");
  get_s_SIM808(4, process_CMTE_resp);
}

float r_temperature_SIM808()
{
  return temperature;
}

static void process_CMTE_resp(char* data)
{
  if(check_msg_header_SIM808(data, "+CMTE") == False)
  {
    return;
  }

  while(data[0] != ',')
  {
    data++;
  }
  data++;

  temperature = ((data[0] - 48) * 10 + (data[1] - 48)) + ((data[3] - 48) * 10 + (data[4] - 48))/100.0;
}

uint16_t r_vcc_SIM808(void)
{
  return vcc;
}

void cmd_vcc_SIM808(void)
{
  put_s_SIM808("AT+CBC\r");
  get_s_SIM808(4, process_CBC_resp);
}

static void process_CBC_resp(char* data)
{
  if(check_msg_header_SIM808(data, "+CBC") == False)
  {
    return;
  }

  uint8_t i = 0;

  while(i != 2)
  {
    if(data[0] == ',')
    {
      i++;
    }

    data++;
  }

  vcc = (data[0] - 48) * 1000 + (data[1] - 48) * 100 + (data[2] - 48) * 10 + (data[3] - 48);
}

boolean_t check_msg_header_SIM808(char* data, char* header)
{
  if(strstr(data, header) == NULL)
  {
    rx_error = True;
    return False;
  }

  return True;
}
