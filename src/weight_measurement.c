/*
 * weight_measurement.c
 *
 *  Created on: 2016. szept. 26.
 *      Author: Bence
 */

#include <weight_measurement.h>
#include <power_management.h>
#include <STM32_bsp/adc.h>
#include <STM32_bsp/constants.h>
#include <STM32_bsp/gpio.h>

#define WINDOW_SIZE 48

static float scale_slope = 2.6; // 2.6 mV / 100 g

static uint16_t unloaded = 0;
static uint16_t filter_buffer[WINDOW_SIZE];

static float average(uint16_t* buf, uint16_t size);
static void push(uint16_t* buffer, uint16_t size, uint16_t element);
static void find_min_max(uint16_t* buf, uint16_t* max, uint16_t* min, uint16_t size);

void scale_init()
{
  uint8_t i;

  unloaded = r_wheatstone_bridge();  // set null point

  for(i = 0; i < WINDOW_SIZE; i++)
  {
    filter_buffer[i] = 65535;
  }
}

uint16_t r_wheatstone_bridge()
{
  float output;
  uint16_t mcu_vdd = calculate_MCU_vcc();
  uint16_t digital_val;

  digital_val = r_single_ext_channel_ADC(STRAIN_GAUGE);
  output = (mcu_vdd/4095.0) * digital_val;

  return output;
}

uint16_t process_bridge_output(uint16_t value)
{
  int16_t temp;
  temp = value - unloaded;

  if(temp < 0)
  {
    temp = 0;
  }

  return temp;
}

float averaging_filter(uint16_t input)
{
  uint16_t i, invalid = 0;
  uint16_t min, max;
  float sum = 0;

  push(filter_buffer, WINDOW_SIZE, input);

  for(i = 0; i < WINDOW_SIZE; i++)
  {
    if(filter_buffer[i] == 65535)
    {
      invalid = 1;
    }

    sum += filter_buffer[i];
  }

  if(invalid)
  {
    return -1;
  }
  else
  {
    find_min_max(filter_buffer, &max, &min, WINDOW_SIZE);
    sum = sum - min - max;
    sum = sum/(WINDOW_SIZE-2);
    return sum;
  }
}

uint32_t mass_in_g(float input)
{
  uint16_t integer = input;
  uint8_t fraction = (input - integer) * 100;
  uint32_t mass;
  float temp = integer + fraction/100.0;

  temp = temp/scale_slope;
  mass = temp;

  return mass * 100;
}

static float average(uint16_t* buf, uint16_t size)
{
  uint16_t i;
  float avg = 0;

  for(i = 0; i < size; i++)
  {
    avg += buf[i];
  }

  return avg/size;
}

static void push(uint16_t* buffer, uint16_t size, uint16_t element)
{
  uint16_t i;

  for(i = size-1; i > 0; i--)
  {
    buffer[i] = buffer[i-1];
  }

  buffer[0] = element;
}

static void find_min_max(uint16_t* buf, uint16_t* max, uint16_t* min, uint16_t size)
{
  uint16_t i = 0;

  *max = 0;
  *min = 10000;

  for(i = 0; i < size; i++)
  {
    if(buf[i] > *max) *max = buf[i];
    if(buf[i] < *min) *min = buf[i];
  }
}


