/*
 * SIM808_device.c
 *
 *  Created on: 2016. szept. 8.
 *      Author: Bence
 */

#include "GPRS_GSM_GPS/SIM808_device.h"
#include "GPRS_GSM_GPS/SIM808_driver.h"

#include "string.h"

static float temperature = 0;
static uint16_t vcc = 0;

static void process_tmp_reading_SIM808(char* data);
static void process_vcc_reading_SIM808(char* data);

void set_netlight(uint8_t value)
{
  char cmd[20];
  sprintf(cmd, "AT+CNETLIGHT=%d\r", value);
  put_s_SIM808(cmd);
}

uint8_t get_netlight()
{

  return 255;
}

void cmd_tmp_SIM808()
{
  put_s_SIM808("AT+CMTE?\r");
  get_s_SIM808(4, process_tmp_reading_SIM808);
}

float r_temperature_SIM808()
{
  return temperature;
}

static void process_tmp_reading_SIM808(char* data)
{
  if(strstr(data, "+CMTE") == NULL)
  {
    rx_error = False;
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
  get_s_SIM808(4, process_vcc_reading_SIM808);
}

static void process_vcc_reading_SIM808(char* data)
{
  if(strstr(data, "+CBC") == NULL)
  {
    rx_error = False;
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
